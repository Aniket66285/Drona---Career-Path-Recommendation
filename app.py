from flask import Flask, request, jsonify, render_template
import os

app = Flask(__name__)

# ── Load careers from career.txt (same as file_handler.cpp) ──────────────────
def load_careers(filename="career.txt"):
    careers = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip().replace('\r', '')
                if not line or line.startswith('#'):
                    continue
                parts = line.split('|')
                if len(parts) < 3:
                    continue
                name        = parts[0].strip()
                skills_part = parts[1].strip()
                category    = parts[2].strip()
                description = parts[3].strip() if len(parts) > 3 else ""

                skill_weights = {}
                skills = []
                for seg in skills_part.split(','):
                    seg = seg.strip()
                    if ':' not in seg:
                        continue
                    sname, weight = seg.split(':', 1)
                    sname = sname.strip().lower()
                    try:
                        w = float(weight.strip())
                        skill_weights[sname] = w
                        skills.append(sname)
                    except:
                        pass

                if name and skills:
                    careers.append({
                        'name': name,
                        'category': category,
                        'description': description,
                        'skills': skills,
                        'skillWeights': skill_weights
                    })
    except Exception as e:
        print(f"[ERROR] Cannot load {filename}: {e}")
    return careers

CAREERS = load_careers()

# ── utils (same as utils.cpp) ─────────────────────────────────────────────────
def grade_of(pct):
    if pct >= 80: return "Excellent"
    if pct >= 50: return "Good"
    return "Gap"

# ── Career scoring (same as career.cpp) ───────────────────────────────────────
def score_career(user_skills, career):
    user_set = set(s.lower() for s in user_skills)
    total = 0.0; matched = 0.0
    matched_skills = []; missing_skills = []
    for sk, w in career['skillWeights'].items():
        total += w
        if sk.lower() in user_set:
            matched += w
            matched_skills.append(sk)
        else:
            missing_skills.append(sk)
    raw = (matched / total * 100.0) if total > 0 else 0.0
    return raw, matched_skills, missing_skills

def get_top3(user_skills, user_interests, careers):
    int_set = set(i.lower() for i in user_interests)
    results = []
    for c in careers:
        raw, matched, missing = score_career(user_skills, c)
        boosted = any(kw in c['name'].lower() or kw in c['category'].lower() for kw in int_set)
        match_pct = min(100.0, raw * (1.15 if boosted else 1.0))
        results.append({
            'careerName':    c['name'],
            'category':      c['category'],
            'description':   c['description'],
            'matchPercent':  round(match_pct, 1),
            'rawPercent':    round(raw, 1),
            'matchedSkills': matched,
            'missingSkills': missing,
            'grade':         grade_of(match_pct),
            'boosted':       boosted
        })
    results.sort(key=lambda x: x['matchPercent'], reverse=True)
    for i, r in enumerate(results[:3]):
        r['rank'] = i + 1
    return results[:3]

# ── Roadmap builder (same as roadmap.cpp) ─────────────────────────────────────
PHASE_TITLES    = ["Foundation & Basics","Core Skill Development","Advanced Techniques","Mastery & Real Projects"]
PHASE_DURATIONS = ["3–5 weeks","4–6 weeks","6–8 weeks","8–12 weeks"]
PHASE_TIPS      = [
    "Start with free online tutorials; build a strong base before moving forward.",
    "Work on guided projects and follow official documentation closely.",
    "Join communities, contribute to open-source, or build portfolio pieces.",
    "Apply everything in a capstone project and prepare for interviews."
]

def build_roadmap(career, user_skills):
    user_set = set(s.lower() for s in user_skills)
    sorted_skills = sorted(career['skillWeights'].items(), key=lambda x: x[1], reverse=True)
    total    = len(sorted_skills)
    per_phase = max(1, (total + 3) // 4)
    phases = []
    for p in range(4):
        start = p * per_phase
        end   = min(start + per_phase, total)
        phase_skills = [{'name': sk, 'known': sk.lower() in user_set} for sk, _ in sorted_skills[start:end]]
        if phase_skills:
            phases.append({
                'number':   p + 1,
                'title':    PHASE_TITLES[p],
                'duration': PHASE_DURATIONS[p],
                'tip':      PHASE_TIPS[p],
                'skills':   phase_skills
            })
    return {
        'careerName': career['name'],
        'overview':   f"A structured {total}-skill path from beginner to job-ready {career['name']}.",
        'phases':     phases
    }

# ── Comparison (same as comparison.cpp) ───────────────────────────────────────
def compare_careers(ca, cb, user_skills):
    user_set = set(s.lower() for s in user_skills)
    raw_a, matched_a, missing_a = score_career(user_skills, ca)
    raw_b, matched_b, missing_b = score_career(user_skills, cb)
    pct_a = round(min(100.0, raw_a), 1)
    pct_b = round(min(100.0, raw_b), 1)
    winner = ca['name'] if pct_a >= pct_b else cb['name']
    gap_a  = len(missing_a); gap_b = len(missing_b)
    win_pct  = max(pct_a, pct_b); lose_pct = min(pct_a, pct_b)

    verdict  = f"{winner} is a stronger match at {int(win_pct)}% vs {int(lose_pct)}%. "
    verdict += f"{ca['name']} has {gap_a} skill gap(s); {cb['name']} has {gap_b} skill gap(s). "
    if   gap_a < gap_b: verdict += f"You'll reach {ca['name']} readiness faster."
    elif gap_b < gap_a: verdict += f"You'll reach {cb['name']} readiness faster."
    else:               verdict += "Both have equal gaps — choose based on passion."

    return {
        'nameA': ca['name'], 'nameB': cb['name'],
        'pctA':  pct_a,      'pctB':  pct_b,
        'winner': winner,    'verdict': verdict,
        'matchedA': matched_a, 'missingA': missing_a,
        'matchedB': matched_b, 'missingB': missing_b,
        'descA': ca['description'], 'descB': cb['description']
    }

# ── Flask routes ──────────────────────────────────────────────────────────────
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/careers', methods=['GET'])
def get_careers():
    return jsonify([c['name'] for c in CAREERS])

@app.route('/recommend', methods=['POST'])
def recommend():
    data      = request.json
    skills    = [s.strip().lower() for s in data.get('skills', [])    if s.strip()]
    interests = [i.strip().lower() for i in data.get('interests', []) if i.strip()]
    if not skills:
        return jsonify({'error': 'Enter at least one skill'}), 400
    return jsonify(get_top3(skills, interests, CAREERS))

@app.route('/roadmap', methods=['POST'])
def roadmap():
    data   = request.json
    name   = data.get('career', '').strip()
    skills = [s.strip().lower() for s in data.get('skills', []) if s.strip()]
    career = next((c for c in CAREERS if c['name'].lower() == name.lower()), None)
    if not career:
        return jsonify({'error': 'Career not found'}), 404
    return jsonify(build_roadmap(career, skills))

@app.route('/compare', methods=['POST'])
def compare():
    data   = request.json
    name_a = data.get('careerA', '').strip()
    name_b = data.get('careerB', '').strip()
    skills = [s.strip().lower() for s in data.get('skills', []) if s.strip()]
    ca = next((c for c in CAREERS if c['name'].lower() == name_a.lower()), None)
    cb = next((c for c in CAREERS if c['name'].lower() == name_b.lower()), None)
    if not ca or not cb:
        return jsonify({'error': 'Career not found'}), 404
    if name_a.lower() == name_b.lower():
        return jsonify({'error': 'Select two different careers'}), 400
    return jsonify(compare_careers(ca, cb, skills))

if __name__ == '__main__':
    app.run(debug=True)
