# ✈️ Travel Agent Skills Pack for Claude Code

A complete set of Claude Code skills that turn Claude into your personal AI travel agent. Drop these into your Claude Code project and get expert-level travel planning inside VS Code.

---

## 📦 Skills Included

| Skill | Command | What It Does |
|-------|---------|-------------|
| **itinerary-planner** | `/itinerary-planner` | Day-by-day trip itineraries with logistics |
| **flight-advisor** | `/flight-advisor` | Airfare strategy, airline tips, booking timing |
| **hotel-scout** | `/hotel-scout` | Hotel recommendations with neighbourhood guides |
| **visa-checker** | `/visa-checker` | Visa requirements + documentation checklists |
| **budget-optimizer** | `/budget-optimizer` | Full trip cost breakdowns + savings tips |
| **travel-packer** | `/travel-packer` | Smart packing lists customised to your trip |

---

## 🚀 Installation

### Option A — Project-level (this project only)

```bash
# From your project root in VS Code terminal
cp -r skills/ .claude/
```

### Option B — Global (available in ALL your Claude Code projects)

```bash
# macOS/Linux
cp -r skills/ ~/.claude/

# Windows (PowerShell)
cp -r skills/ $env:USERPROFILE\.claude\
```

### Option C — VS Code (GitHub Copilot / Agent Skills)

```bash
# Create the skills folder in your workspace
mkdir -p .github/skills
cp -r skills/* .github/skills/
```

---

## 💬 How to Use

Skills activate **automatically** when Claude detects a relevant task, or you can invoke them explicitly:

```
# Auto-trigger examples (just type naturally)
"Plan a 5-day trip to Bali for 2 people in July"
"What visa do I need for France with an Indian passport?"
"How much will a trip to Japan cost?"

# Explicit invocation
/itinerary-planner Plan 7 days in Kyoto, culture focus, mid-range budget
/visa-checker Indian passport → Canada tourist visa
/travel-packer Goa beach trip, 4 days, carry-on only
/budget-optimizer 10 days in Europe for 2 from Delhi
/hotel-scout Best area to stay in Bangkok for first-timers
/flight-advisor Cheapest way to fly Delhi to New York in December
```

---

## 📁 Package Structure

```
travel-agent-skills/
├── README.md
└── skills/
    ├── itinerary-planner/
    │   └── SKILL.md
    ├── flight-advisor/
    │   └── SKILL.md
    ├── hotel-scout/
    │   └── SKILL.md
    ├── visa-checker/
    │   └── SKILL.md
    ├── budget-optimizer/
    │   └── SKILL.md
    └── travel-packer/
        └── SKILL.md
```

---

## 🧠 Skill Trigger Cheatsheet

| You say... | Skill activated |
|-----------|----------------|
| "plan a trip / itinerary / what to do in" | itinerary-planner |
| "flights / airline / airfare / cheapest to fly" | flight-advisor |
| "hotel / where to stay / accommodation" | hotel-scout |
| "visa / passport / entry requirements" | visa-checker |
| "budget / cost / how much / expensive" | budget-optimizer |
| "pack / what to bring / packing list" | travel-packer |

---

## ⚙️ Requirements

- Claude Code (Pro, Max, Team, or Enterprise plan)
- Code execution enabled in Settings → Capabilities
- Claude Code VS Code extension installed

---

## 📝 Notes

- Skills are **private to your account** — not shared
- All budget figures default to INR (₹) context for India-based users; Claude adapts to other currencies automatically
- Visa information is advisory only — always verify with official embassy sites
