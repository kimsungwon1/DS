"""
DS Battle HUD - Widget Blueprint Creator v4
KEY FIXES:
- Use factory CanvasPanel root (never replace root_widget)
- Use add_child() for all parent-child (NOT set_content - that's runtime only)
- Removed compile_blueprint (it auto-opens editor which resets the tree)
Run: py "D:/ueg/DS/Content/Python/CreateDSHudWidgets.py"
"""
import unreal

ASSET_PATH   = "/Game/UI"
FORCE_RECREATE = True

C_BG       = unreal.LinearColor(0.03, 0.02, 0.01, 1.0)
C_BORDER_N = unreal.LinearColor(0.15, 0.12, 0.06, 1.0)
C_GOLD     = unreal.LinearColor(0.90, 0.72, 0.18, 1.0)
C_PARCHM   = unreal.LinearColor(0.85, 0.80, 0.65, 1.0)
C_DIM      = unreal.LinearColor(0.55, 0.50, 0.40, 1.0)
C_HP       = unreal.LinearColor(0.12, 0.70, 0.12, 1.0)
C_SP       = unreal.LinearColor(0.10, 0.28, 0.85, 1.0)
C_EHPO     = unreal.LinearColor(0.80, 0.10, 0.10, 1.0)
C_DEAD     = unreal.LinearColor(0.85, 0.08, 0.08, 1.0)
C_LOG      = unreal.LinearColor(0.75, 0.70, 0.55, 1.0)
C_STATUS   = unreal.LinearColor(1.00, 0.88, 0.25, 1.0)

HA = unreal.HorizontalAlignment
VA = unreal.VerticalAlignment


# ── helpers ──────────────────────────────────────────────────────────

def ensure_dir():
    if not unreal.EditorAssetLibrary.does_directory_exist(ASSET_PATH):
        unreal.EditorAssetLibrary.make_directory(ASSET_PATH)


def make_bp(name, parent_path):
    full = f"{ASSET_PATH}/{name}.{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        if FORCE_RECREATE:
            unreal.log(f"  [del] {name}")
            unreal.EditorAssetLibrary.delete_asset(full)
        else:
            unreal.log(f"  [skip] {name}")
            return unreal.load_asset(full)
    parent = unreal.load_class(None, parent_path)
    if not parent:
        unreal.log_error(f"  Cannot load parent: {parent_path}")
        return None
    factory = unreal.WidgetBlueprintFactory()
    factory.set_editor_property("parent_class", parent)
    bp = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name, ASSET_PATH, unreal.WidgetBlueprint, factory
    )
    if not bp:
        unreal.log_error(f"  create_asset failed: {name}")
    return bp


def get_wt(bp):
    try:
        wt = bp.get_editor_property("widget_tree")
        if wt:
            return wt
    except Exception as e:
        unreal.log_warning(f"  widget_tree err: {e}")
    return None


def get_root_canvas(wt):
    """Return the factory-created CanvasPanel root (do NOT replace root_widget)."""
    root = None
    try:
        root = wt.get_editor_property("root_widget")
    except Exception as e:
        unreal.log_warning(f"  root_widget get err: {e}")
    if root is not None:
        unreal.log(f"  root: {type(root).__name__}")
        if isinstance(root, unreal.CanvasPanel):
            return root
        # Unexpected root type — construct a canvas on top
        unreal.log_warning(f"  root is not CanvasPanel, wrapping...")
    canvas = wt.construct_widget(unreal.CanvasPanel, "HUD_RootCanvas")
    try:
        wt.set_editor_property("root_widget", canvas)
    except Exception as e:
        unreal.log_warning(f"  set root_widget: {e}")
    return canvas


def w(widget):
    """Shortcut — call modify on a widget so changes are tracked."""
    if widget:
        try:
            widget.modify(True)
        except Exception:
            pass
    return widget


def add(parent, child):
    """add_child with error guard. Returns slot."""
    if parent is None or child is None:
        return None
    try:
        return parent.add_child(child)
    except Exception as e:
        unreal.log_warning(f"  add_child {type(parent).__name__}←{type(child).__name__}: {e}")
        return None


def txt(wt, name, text, color=None):
    t = wt.construct_widget(unreal.TextBlock, name)
    if t is None:
        unreal.log_error(f"  TextBlock '{name}' failed")
        return None
    w(t)
    t.set_editor_property("text", unreal.Text(text))
    if color:
        try:
            sc = unreal.SlateColor()
            sc.set_editor_property("color_use_rule", unreal.SlateColorStylingMode.USE_COLOR_SPECIFIED)
            sc.set_editor_property("specified_color", color)
            t.set_editor_property("color_and_opacity", sc)
        except Exception as e:
            unreal.log_warning(f"  TextBlock color: {e}")
    return t


def bar(wt, name, fill_color):
    b = wt.construct_widget(unreal.ProgressBar, name)
    if b is None:
        unreal.log_error(f"  ProgressBar '{name}' failed")
        return None
    w(b)
    try:
        b.set_editor_property("percent", 1.0)
        b.set_editor_property("fill_color_and_opacity", fill_color)
    except Exception as e:
        unreal.log_warning(f"  ProgressBar props: {e}")
    return b


def sz(fill):
    s = unreal.SlateChildSize()
    s.set_editor_property("size_rule", unreal.SlateSizeRule.FILL if fill else unreal.SlateSizeRule.AUTOMATIC)
    s.set_editor_property("value", 1.0 if fill else 0.0)
    return s


def vslot(slot, fill=False, pad=2, h=HA.H_ALIGN_FILL):
    if slot is None:
        return
    m = pad if isinstance(pad, unreal.Margin) else unreal.Margin(pad, pad, pad, pad)
    try:
        slot.set_editor_property("padding", m)
        slot.set_editor_property("horizontal_alignment", h)
        slot.set_editor_property("size", sz(fill))
    except Exception as e:
        unreal.log_warning(f"  vslot: {e}")


def hslot(slot, fill=False, pad=2, v=VA.V_ALIGN_CENTER):
    if slot is None:
        return
    m = pad if isinstance(pad, unreal.Margin) else unreal.Margin(pad, pad, pad, pad)
    try:
        slot.set_editor_property("padding", m)
        slot.set_editor_property("vertical_alignment", v)
        slot.set_editor_property("size", sz(fill))
    except Exception as e:
        unreal.log_warning(f"  hslot: {e}")


def cadd(canvas, widget, ax0, ay0, ax1, ay1, l, t, r, b_, auto=False):
    """Add widget to CanvasPanel with anchor/offset."""
    if widget is None:
        return None
    try:
        slot = canvas.add_child(widget)
        slot.set_editor_property("anchors", unreal.Anchors(ax0, ay0, ax1, ay1))
        slot.set_editor_property("offsets", unreal.Margin(l, t, r, b_))
        slot.set_editor_property("auto_size", auto)
        return slot
    except Exception as e:
        unreal.log_warning(f"  cadd: {e}")
        return None


def save_bp(bp):
    try:
        unreal.EditorAssetLibrary.save_loaded_asset(bp)
        unreal.log(f"  Saved")
    except Exception as e:
        unreal.log_error(f"  Save failed: {e}")


# ── 1. WBP_PartyMember ───────────────────────────────────────────────
def create_party_member():
    unreal.log("[1/5] WBP_PartyMember")
    bp = make_bp("WBP_PartyMember", "/Script/DS.DSPartyMemberWidget")
    if not bp: return None
    w(bp)
    wt = get_wt(bp)
    if not wt: save_bp(bp); return bp
    w(wt)

    canvas = get_root_canvas(wt)  # factory CanvasPanel — do NOT replace
    if not canvas: save_bp(bp); return bp
    w(canvas)

    # SelectionBorder fills entire canvas
    border = wt.construct_widget(unreal.Border, "SelectionBorder"); w(border)
    border.set_editor_property("brush_color", C_BORDER_N)
    border.set_editor_property("padding", unreal.Margin(3, 3, 3, 3))
    cadd(canvas, border, 0, 0, 1, 1, 0, 0, 0, 0)

    # VerticalBox inside border — use add_child (NOT set_content)
    vbox = wt.construct_widget(unreal.VerticalBox, "PM_VBox"); w(vbox)
    add(border, vbox)

    portrait = wt.construct_widget(unreal.Image, "PortraitImage"); w(portrait)
    vslot(add(vbox, portrait), fill=True, pad=unreal.Margin(1,1,1,1))

    name_tb = txt(wt, "CharacterNameText", "CHARACTER", C_GOLD)
    vslot(add(vbox, name_tb), pad=unreal.Margin(3,2,3,0), h=HA.H_ALIGN_CENTER)

    prof_tb = txt(wt, "ProfessionText", "Fighter", C_DIM)
    vslot(add(vbox, prof_tb), pad=unreal.Margin(3,0,3,3), h=HA.H_ALIGN_CENTER)

    hp_bar = bar(wt, "HPBar", C_HP)
    vslot(add(vbox, hp_bar), pad=unreal.Margin(3,1,3,0))

    hp_tb = txt(wt, "HPText", "HP 100/100", C_PARCHM)
    vslot(add(vbox, hp_tb), pad=unreal.Margin(3,0,3,2), h=HA.H_ALIGN_RIGHT)

    sp_bar = bar(wt, "SPBar", C_SP)
    vslot(add(vbox, sp_bar), pad=unreal.Margin(3,1,3,0))

    sp_tb = txt(wt, "SPText", "SP  50/50", C_PARCHM)
    vslot(add(vbox, sp_tb), pad=unreal.Margin(3,0,3,2), h=HA.H_ALIGN_RIGHT)

    dead_ov = wt.construct_widget(unreal.Overlay, "DeadOverlay"); w(dead_ov)
    dead_ov.set_editor_property("visibility", unreal.SlateVisibility.COLLAPSED)
    dead_lbl = txt(wt, "Dead_Label", "† DEAD †", C_DEAD)
    d_slot = add(dead_ov, dead_lbl)
    if d_slot:
        try:
            d_slot.set_editor_property("horizontal_alignment", HA.H_ALIGN_CENTER)
            d_slot.set_editor_property("vertical_alignment", VA.V_ALIGN_CENTER)
        except Exception: pass
    vslot(add(vbox, dead_ov), pad=unreal.Margin(0))

    save_bp(bp)
    unreal.log("  OK")
    return bp


# ── 2. WBP_ActionButton ──────────────────────────────────────────────
def create_action_button():
    unreal.log("[2/5] WBP_ActionButton")
    bp = make_bp("WBP_ActionButton", "/Script/DS.DSActionButtonWidget")
    if not bp: return None
    w(bp)
    wt = get_wt(bp)
    if not wt: save_bp(bp); return bp
    w(wt)

    canvas = get_root_canvas(wt)
    if not canvas: save_bp(bp); return bp
    w(canvas)

    btn = wt.construct_widget(unreal.Button, "ActionButton"); w(btn)
    btn.set_editor_property("background_color", C_BG)
    cadd(canvas, btn, 0, 0, 1, 1, 0, 0, 0, 0)

    hbox = wt.construct_widget(unreal.HorizontalBox, "AB_HBox"); w(hbox)
    add(btn, hbox)  # add_child, NOT set_content

    action_tb = txt(wt, "ActionNameText", "Attack", C_PARCHM)
    hslot(add(hbox, action_tb), fill=True, pad=unreal.Margin(8,4,4,4))

    cost_tb = txt(wt, "ActionCostText", "0 SP", C_SP)
    hslot(add(hbox, cost_tb), fill=False, pad=unreal.Margin(4,4,8,4))

    save_bp(bp)
    unreal.log("  OK")
    return bp


# ── 3. WBP_ActionPanel ───────────────────────────────────────────────
def create_action_panel():
    unreal.log("[3/5] WBP_ActionPanel")
    bp = make_bp("WBP_ActionPanel", "/Script/DS.DSActionPanelWidget")
    if not bp: return None
    w(bp)
    wt = get_wt(bp)
    if not wt: save_bp(bp); return bp
    w(wt)

    canvas = get_root_canvas(wt)
    if not canvas: save_bp(bp); return bp
    w(canvas)

    vbox = wt.construct_widget(unreal.VerticalBox, "AP_VBox"); w(vbox)
    cadd(canvas, vbox, 0, 0, 1, 1, 0, 0, 0, 0)

    header = txt(wt, "ActiveCharacterNameText", "── Select Action ──", C_GOLD)
    vslot(add(vbox, header), pad=unreal.Margin(4,6,4,4), h=HA.H_ALIGN_CENTER)

    scroll = wt.construct_widget(unreal.ScrollBox, "ActionListBox"); w(scroll)
    try:
        scroll.set_editor_property("orientation", unreal.Orientation.ORIENT_VERTICAL)
    except Exception as e:
        unreal.log_warning(f"  ScrollBox orient: {e}")
    vslot(add(vbox, scroll), fill=True, pad=unreal.Margin(2,0,2,2))

    save_bp(bp)
    unreal.log("  OK")
    return bp


# ── 4. WBP_EnemyInfo ─────────────────────────────────────────────────
def create_enemy_info():
    unreal.log("[4/5] WBP_EnemyInfo")
    bp = make_bp("WBP_EnemyInfo", "/Script/DS.DSEnemyInfoWidget")
    if not bp: return None
    w(bp)
    wt = get_wt(bp)
    if not wt: save_bp(bp); return bp
    w(wt)

    canvas = get_root_canvas(wt)
    if not canvas: save_bp(bp); return bp
    w(canvas)

    hbox = wt.construct_widget(unreal.HorizontalBox, "EI_HBox"); w(hbox)
    cadd(canvas, hbox, 0, 0, 1, 1, 0, 0, 0, 0)

    name_tb = txt(wt, "EnemyNameText", "Dragon", C_EHPO)
    hslot(add(hbox, name_tb), fill=False, pad=unreal.Margin(12,4,8,4))

    hp_bar = bar(wt, "EnemyHPBar", C_EHPO)
    hslot(add(hbox, hp_bar), fill=True, pad=unreal.Margin(4,10,4,10))

    hp_text = txt(wt, "EnemyHPText", "500/500", C_PARCHM)
    hslot(add(hbox, hp_text), fill=False, pad=unreal.Margin(8,4,12,4))

    save_bp(bp)
    unreal.log("  OK")
    return bp


def load_wbp_cls(asset_path):
    name = asset_path.split("/")[-1]
    return unreal.load_class(None, f"{asset_path}.{name}_C")


# ── 5. WBP_BattleHUD ─────────────────────────────────────────────────
def create_battle_hud():
    unreal.log("[5/5] WBP_BattleHUD")
    bp = make_bp("WBP_BattleHUD", "/Script/DS.DSBattleHUDWidget")
    if not bp: return None
    w(bp)
    wt = get_wt(bp)
    if not wt: save_bp(bp); return bp
    w(wt)

    canvas = get_root_canvas(wt)
    if not canvas: save_bp(bp); return bp
    w(canvas)

    # EnemyInfoWidget — top strip full width 70 px
    ei_cls = load_wbp_cls("/Game/UI/WBP_EnemyInfo")
    ei = wt.construct_widget(ei_cls if ei_cls else unreal.Border, "EnemyInfoWidget"); w(ei)
    cadd(canvas, ei, 0, 0, 1, 0, 0, 0, 0, 70)

    # CombatStatusText — centered
    status_tb = txt(wt, "CombatStatusText", "", C_STATUS)
    s = cadd(canvas, status_tb, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0, auto=True)
    if s:
        try:
            s.set_editor_property("alignment", unreal.Vector2D(0.5, 0.5))
        except Exception: pass

    # ActionPanelWidget — right 220 px
    ap_cls = load_wbp_cls("/Game/UI/WBP_ActionPanel")
    ap = wt.construct_widget(ap_cls if ap_cls else unreal.Border, "ActionPanelWidget"); w(ap)
    cadd(canvas, ap, 1, 0, 1, 1, -220, 70, 220, 130)

    # CombatLogBorder — lower-left 78 %
    log_border = wt.construct_widget(unreal.Border, "CombatLogBorder"); w(log_border)
    log_border.set_editor_property("brush_color", unreal.LinearColor(0.04, 0.03, 0.02, 0.90))
    log_border.set_editor_property("padding", unreal.Margin(8, 6, 8, 6))
    log_tb = txt(wt, "CombatLogText", "", C_LOG)
    add(log_border, log_tb)  # add_child
    cadd(canvas, log_border, 0, 1, 0.78, 1, 0, -290, 0, 160)

    # PartyMemberBox — bottom full-width 130 px
    party_hbox = wt.construct_widget(unreal.HorizontalBox, "PartyMemberBox"); w(party_hbox)
    cadd(canvas, party_hbox, 0, 1, 1, 1, 0, -130, 0, 130)

    save_bp(bp)
    unreal.log("  OK")
    return bp


# ═══ MAIN ════════════════════════════════════════════════════════════
unreal.log("=== DS HUD Widget Creator v4 ===")
ensure_dir()

create_party_member()
create_action_button()
create_action_panel()
create_enemy_info()
create_battle_hud()

unreal.log("=== Done! ===")
unreal.log("Now open each WBP in editor and click [Compile] button manually.")
unreal.log("WBP_ActionPanel  -> Details: ActionButtonClass     = WBP_ActionButton")
unreal.log("WBP_BattleHUD   -> Details: PartyMemberWidgetClass = WBP_PartyMember")
