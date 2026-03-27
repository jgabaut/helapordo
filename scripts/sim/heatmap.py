import csv
import re
from collections import defaultdict

filename = "res.csv"

# --- ANSI escape regex ---
ansi_escape = re.compile(r'\033\[[0-9;]*m')

# --- Data structures ---
# Each level maps (player_class, enemy_class) -> stats dict
data_by_level = defaultdict(dict)

# --- Load CSV ---
with open(filename, newline="") as f:
    reader = csv.DictReader(f)
    for row in reader:
        pclass = int(row["player_class"])
        eclass = int(row["enemy_class"])
        plevel = int(row["player_level"])
        wins = int(row["wins"])
        total = int(row["total_fights"])
        rate = wins / total if total else 0

        # Store all relevant stats for this matchup
        data_by_level[plevel][(pclass, eclass)] = {
            "win_rate": rate,
            "player_hp": int(row["player_hp_start"]),
            "enemy_hp": int(row["enemy_hp_start"]),
            "player_attack": int(row["player_attack"]),
            "enemy_attack": int(row["enemy_attack"]),
            "player_def": int(row["player_def"]),
            "enemy_def": int(row["enemy_def"]),
            "player_vel": int(row["player_vel"]),
            "enemy_vel": int(row["enemy_vel"]),
        }

player_names = ["Knight", "Archer", "Mage", "Assassin"]
enemy_names  = ["Mummy", "Ghost", "Zombie", "Goblin", "Imp", "Troll", "Boar", "Werewolf"]

threshold_winrate_jump = 0.2  # >20% jump flagged as critical
threshold_stat_delta = 2       # vel or atk or def delta considered significant

for p in range(len(player_names)):
    for e in range(len(enemy_names)):
        last_wr = None
        for lvl in sorted(data_by_level.keys()):
            stats = data_by_level[lvl].get((p, e))
            if not stats:
                continue
            wr = stats["win_rate"]
            vel = stats["player_vel"] - stats["enemy_vel"]
            atk = stats["player_attack"] - stats["enemy_attack"]
            df = stats["player_def"] - stats["enemy_def"]

            if last_wr is not None:
                delta_wr = wr - last_wr
                if delta_wr >= threshold_winrate_jump and (
                    abs(vel) >= threshold_stat_delta or
                    abs(atk) >= threshold_stat_delta or
                    abs(df) >= threshold_stat_delta
                ):
                    print(f"Breakpoint detected: {player_names[p]} vs {enemy_names[e]} "
                          f"Level {lvl} | ΔWR={delta_wr:.2f} | Δvel={vel} Δatk={atk} Δdef={df}")
            last_wr = wr

col_width = 12

# --- color helper ---
def color_rate(rate):
    s = f"{rate:5.2f}"
    if rate < 0.4:
        return f"\033[1;31m{s}\033[0m"
    elif rate < 0.6:
        return f"\033[1;33m{s}\033[0m"
    else:
        return f"\033[1;32m{s}\033[0m"

def color_diff(val):
    if val < 0:
        return f"\033[1;31m{val:3d}\033[0m"
    elif val == 0:
        return f"\033[1;33m{val:3d}\033[0m"
    else:
        return f"\033[1;32m{val:3d}\033[0m"

# --- pad string while ignoring ANSI codes ---
def pad_ansi(s, width, align='right'):
    visible_len = len(ansi_escape.sub('', s))
    pad_len = width - visible_len
    if pad_len <= 0:
        return s
    if align == 'right':
        return ' ' * pad_len + s
    elif align == 'left':
        return s + ' ' * pad_len
    else:  # center
        left = pad_len // 2
        right = pad_len - left
        return ' ' * left + s + ' ' * right

# --- Print Winrate heatmaps ---
for lvl in sorted(data_by_level.keys()):
    print(f"\n=== Player Level {lvl} ===")

    # header
    header = ' ' * col_width
    for e in enemy_names:
        header += f"{e:>{col_width}}"
    header += f"{"Avg WR":>{col_width}}"
    print(header)

    # rows
    for p in range(len(player_names)):
        row = f"{player_names[p]:<{col_width}}"
        total_wr = 0
        for e in range(len(enemy_names)):
            stats = data_by_level[lvl].get((p, e), None)
            if stats:
                rate = stats["win_rate"]
                row += pad_ansi(color_rate(rate), col_width)
                total_wr += rate
            else:
                row += ' ' * col_width
        avg_wr = total_wr / len(enemy_names)
        row += pad_ansi(color_rate(avg_wr), col_width)
        print(row)

# --- Combined delta heatmap ---
print("\n--- Player vs Enemy Stat Deltas (vel/atk/def) ---")
for lvl in sorted(data_by_level.keys()):
    print(f"\n=== Player Level {lvl} ===")

    # header
    header = ' ' * col_width
    for e in enemy_names:
        header += f"{e:>{col_width}}"
    print(header)

    for p in range(len(player_names)):
        row = f"{player_names[p]:<{col_width}}"
        for e in range(len(enemy_names)):
            stats = data_by_level[lvl].get((p, e), None)
            if stats:
                vel = stats["player_vel"] - stats["enemy_vel"]
                atk = stats["player_attack"] - stats["enemy_attack"]
                df  = stats["player_def"] - stats["enemy_def"]

                cell = f"{color_diff(vel)}/{color_diff(atk)}/{color_diff(df)}"
                row += pad_ansi(cell, col_width)
            else:
                row += ' ' * col_width
        print(row)

# --- Heatmap: Player vs Enemy Combined Stat Impact (vel + atk + def) with dominant stat ---
print("\n--- Player vs Enemy Stat Impact (vel + atk + def, dominant stat) ---")

for lvl in sorted(data_by_level.keys()):
    print(f"\n=== Player Level {lvl} ===")

    # header
    header = ' ' * col_width
    for e in enemy_names:
        header += f"{e:>{col_width}}"
    print(header)

    for p in range(len(player_names)):
        row = f"{player_names[p]:<{col_width}}"
        for e in range(len(enemy_names)):
            stats = data_by_level[lvl].get((p, e), None)
            if stats:
                vel, atk, df = stats["player_vel"] - stats["enemy_vel"], \
                                stats["player_attack"] - stats["enemy_attack"], \
                                stats["player_def"] - stats["enemy_def"]
                total = vel + atk + df

                # Determine dominant stat by absolute value
                dom = max([(abs(vel), 'V'), (abs(atk), 'A'), (abs(df), 'D')], key=lambda x: x[0])[1]

                # color by total (red=low, yellow=medium, green=high) similar to WR
                if total < 0:
                    s = f"\033[1;31m{total:3d}({dom})\033[0m"
                elif total < 15:
                    s = f"\033[1;33m{total:3d}({dom})\033[0m"
                else:
                    s = f"\033[1;32m{total:3d}({dom})\033[0m"

                row += pad_ansi(s, col_width)
            else:
                row += ' ' * col_width
        print(row)

# --- Heatmap: Fight Branch per Matchup ---
print("\n--- Player vs Enemy Fight Branch ---")
for lvl in sorted(data_by_level.keys()):
    print(f"\n=== Player Level {lvl} ===")

    # header
    header = ' ' * col_width
    for e in enemy_names:
        header += f"{e:>{col_width}}"
    print(header)

    for p in range(len(player_names)):
        row = f"{player_names[p]:<{col_width}}"
        for e in range(len(enemy_names)):
            stats = data_by_level[lvl].get((p, e), None)
            if stats:
                # Compute deltas
                veldelta = stats["player_vel"] - stats["enemy_vel"]
                atkOnEnemy = stats["player_attack"] - stats["enemy_def"] - (stats["enemy_vel"] // 6)
                atkOnPlayer = stats["enemy_attack"] - (stats["player_def"] + stats["player_vel"] // 6)

                # Determine branch using your fight() logic
                if veldelta >= 0:
                    if atkOnEnemy > 3:
                        branch = 'A'
                    elif atkOnEnemy >= 0:
                        branch = 'B'
                    elif atkOnEnemy > -3:
                        branch = 'C1'
                    else:
                        branch = 'C2'
                else:
                    if atkOnPlayer > 3:
                        branch = 'D'
                    elif atkOnPlayer >= 0:
                        branch = 'E'
                    elif atkOnPlayer > -3:
                        branch = 'F1'
                    else:
                        branch = 'F2'

                row += pad_ansi(f"{branch:>{col_width}}", col_width)
            else:
                row += ' ' * col_width
        print(row)
