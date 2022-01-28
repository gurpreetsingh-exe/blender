/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file
 * \ingroup spassets
 */

#include <cstring>

#include "DNA_screen_types.h"
#include "DNA_space_types.h"

#include "BKE_screen.h"

#include "BLI_listbase.h"

#include "ED_screen.h"
#include "ED_space_api.h"

#include "MEM_guardedalloc.h"

#include "UI_resources.h"

#include "asset_browser_intern.hh"

/* ---------------------------------------------------------------------- */
/* Asset Browser Space */

static SpaceLink *asset_browser_create(const ScrArea *UNUSED(area), const Scene *UNUSED(scene))
{
  SpaceAssets *assets_space = MEM_cnew<SpaceAssets>("asset browser space");
  assets_space->spacetype = SPACE_ASSETS;

  {
    /* Header. */
    ARegion *region = MEM_cnew<ARegion>("asset browser header");
    BLI_addtail(&assets_space->regionbase, region);
    region->regiontype = RGN_TYPE_HEADER;
    region->alignment = (U.uiflag & USER_HEADER_BOTTOM) ? RGN_ALIGN_BOTTOM : RGN_ALIGN_TOP;
  }

  {
    /* Main window. */
    ARegion *region = MEM_cnew<ARegion>("asset browser main region");
    BLI_addtail(&assets_space->regionbase, region);
    region->regiontype = RGN_TYPE_WINDOW;
  }

  return (SpaceLink *)assets_space;
}

static void asset_browser_free(SpaceLink *UNUSED(sl))
{
}

static void asset_browser_init(wmWindowManager *UNUSED(wm), ScrArea *UNUSED(area))
{
}

static SpaceLink *asset_browser_duplicate(SpaceLink *sl)
{
  const SpaceAssets *asset_browser_old = (SpaceAssets *)sl;
  SpaceAssets *asset_browser_new = reinterpret_cast<SpaceAssets *>(
      MEM_dupallocN(asset_browser_old));

  return (SpaceLink *)asset_browser_new;
}

static void asset_browser_keymap(wmKeyConfig *UNUSED(keyconf))
{
}

/* ---------------------------------------------------------------------- */
/* Main Region */

static void asset_browser_main_region_init(wmWindowManager *UNUSED(wm), ARegion *UNUSED(region))
{
}

static void asset_browser_main_region_draw(const bContext *UNUSED(C), ARegion *UNUSED(region))
{
  UI_ThemeClearColor(TH_BACK);
}

static void asset_browser_main_region_listener(const wmRegionListenerParams *UNUSED(params))
{
}

/* ---------------------------------------------------------------------- */
/* Header Region */

static void asset_browser_header_init(wmWindowManager *UNUSED(wm), ARegion *region)
{
  ED_region_header_init(region);
}

static void asset_browser_header_listener(const wmRegionListenerParams *UNUSED(params))
{
}

/* ---------------------------------------------------------------------- */
/* Asset Browser Space-Type */

void ED_spacetype_assets(void)
{
  SpaceType *st = MEM_cnew<SpaceType>("spacetype asset browser");
  ARegionType *art;

  st->spaceid = SPACE_ASSETS;
  strncpy(st->name, "Asset Browser", BKE_ST_MAXNAME);

  st->create = asset_browser_create;
  st->free = asset_browser_free;
  st->init = asset_browser_init;
  st->duplicate = asset_browser_duplicate;
  st->operatortypes = asset_browser_operatortypes;
  st->keymap = asset_browser_keymap;

  /* regions: main window */
  art = MEM_cnew<ARegionType>("spacetype asset browser main region");
  art->regionid = RGN_TYPE_WINDOW;
  art->init = asset_browser_main_region_init;
  art->draw = asset_browser_main_region_draw;
  art->listener = asset_browser_main_region_listener;
  art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_HEADER;
  BLI_addhead(&st->regiontypes, art);

  /* regions: header */
  art = MEM_cnew<ARegionType>("spacetype asset browser header region");
  art->regionid = RGN_TYPE_HEADER;
  art->prefsizey = HEADERY;
  art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_HEADER;
  art->listener = asset_browser_header_listener;
  art->init = asset_browser_header_init;
  art->layout = ED_region_header_layout;
  art->draw = ED_region_header_draw;
  BLI_addhead(&st->regiontypes, art);

  BKE_spacetype_register(st);
}