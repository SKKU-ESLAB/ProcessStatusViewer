/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "processstatusviewer.h"

/* Menu Configuration START */
#define MENU_NUM 2
void total_process_list_cb(void *data, Evas_Object *obj, void *event_info);
void process_tree_cb(void *data, Evas_Object *obj, void *event_info);

const char gMenuNames[MENU_NUM][50] = { "Total Process List",
		"Process Tree" };

Evas_Smart_Cb gMenuCallbacks[MENU_NUM] = { total_process_list_cb,
		process_tree_cb };
/* Menu Configuration END */

/* App Evas Objects: It contains Evas objects, which is main layout of the application */
typedef struct app_evas_objs {
	Evas_Object *mWindow;
	Evas_Object *mConformant;
	Evas_Object *mNaviFrame;
	Evas_Object *mBaseLayout;
	Evas_Object *mPanel;
	Evas_Object *mPanelList;
	Evas_Object *mGenList;
} app_eobjs_s;

/* Process Data */
typedef struct process_info_evas_objs {
	Evas_Object *to;
	Evas_Object *subject;
	Evas_Object *content;
	Evas_Object *popup;

	app_eobjs_s *mAppEobjs;
} proc_info_eobjs_s;

/* List Item Data */
typedef struct list_item_evas_objs {
	int index;
	Elm_Object_Item *mItem;

	app_eobjs_s *mAppEobjs;
} list_item_eobjs_s;

static void win_delete_request_cb(void *data, Evas_Object *obj,
		void *event_info) {
	ui_app_exit();
}

static Evas_Object*
create_favorite_icon(Evas_Object *parent) {
	Evas_Object *img = elm_image_add(parent);
	char buf[PATH_MAX];

	snprintf(buf, sizeof(buf), "%s/favorite.png", ICON_DIR);
	elm_image_file_set(img, buf, NULL);
	evas_object_color_set(img, 255, 180, 0, 255);
	evas_object_size_hint_align_set(img, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(img, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	return img;
}

static Eina_Bool nf_it_pop_cb(void *data, Elm_Object_Item *it) {
	/* Don't pop the last view but hide window */
	app_eobjs_s *ad = data;
	elm_win_lower(ad->mWindow);
	return EINA_FALSE;
}

static void back_button_clicked_cb(void *data, Evas_Object * obj,
		void *event_info) {
	proc_info_eobjs_s *cd = data;
	app_eobjs_s *ad = cd->mAppEobjs;
	elm_naviframe_item_pop(ad->mNaviFrame);
}

static Evas_Object *
create_button(Evas_Object *parent, char *style, char *text) {
	Evas_Object *button;

	button = elm_button_add(parent);
	if (style)
		elm_object_style_set(button, style);
	if (text)
		elm_object_text_set(button, text);
	evas_object_show(button);

	return button;
}

static Evas_Object*
create_scroller(Evas_Object *parent) {
	Evas_Object *scroller;

	scroller = elm_scroller_add(parent);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF,
			ELM_SCROLLER_POLICY_AUTO);
	evas_object_show(scroller);

	return scroller;
}

static Evas_Object*
create_box(Evas_Object *parent) {
	Evas_Object *box;
	box = elm_box_add(parent);
	evas_object_show(box);

	return box;
}

static Evas_Object*
create_entry(Evas_Object *parent, char *text, char *guide) {
	Evas_Object *entry;
	entry = elm_entry_add(parent);

	if (text)
		elm_object_text_set(entry, text);
	if (guide)
		elm_object_part_text_set(entry, "elm.guide", guide);
	evas_object_show(entry);

	return entry;
}

static void left_panel_button_clicked_cb(void *data, Evas_Object * obj,
		void *event_info) {
	app_eobjs_s *ad = data;
	if (!elm_object_disabled_get(ad->mPanel))
		elm_panel_toggle(ad->mPanel);
}

static void panel_scroll_cb(void *data, Evas_Object *obj, void *event_info) {
	Elm_Panel_Scroll_Info *ev = event_info;
	Evas_Object *bg = data;
	int col = 127 * ev->rel_x;
	evas_object_color_set(bg, 0, 0, 0, col);
}

static void list_clicked_cb(void *data, Evas_Object *obj, void *event_info) {
	app_eobjs_s *appEobjs = data;
	Elm_Object_Item *it = (Elm_Object_Item *) elm_list_selected_item_get(obj);
	if (it == NULL)
		return;
	elm_object_item_part_text_set(elm_naviframe_top_item_get(appEobjs->mNaviFrame), NULL,
			elm_object_item_part_text_get(it, "default"));
	elm_list_item_selected_set(it, EINA_FALSE);
}

static void list_back_cb(void *data, Evas_Object *obj, void *event_info) {
	Evas_Object *panel = data;
	elm_panel_hidden_set(panel, EINA_TRUE);
}

void total_process_list_cb(void *data, Evas_Object *obj, void *event_info) {
	/* Add your application logic. */
}

void process_tree_cb(void *data, Evas_Object *obj, void *event_info) {
	/* Add your application logic. */
}

static void gl_del_cb(void *data, Evas_Object *obj EINA_UNUSED) {
	/* FIXME: Unrealized callback can be called after this. */
	/* Accessing list_item_eobjs_s can be dangerous on unrealized callback. */
	list_item_eobjs_s *listItemEobjs = data;
	free(listItemEobjs);
}

static char *
gl_text_get_cb(void *data, Evas_Object *obj, const char *part) {
	list_item_eobjs_s *listItemEobjs = data;
	const Elm_Genlist_Item_Class *genlistItemClass = elm_genlist_item_item_class_get(
			listItemEobjs->mItem);
	char buf[1024];

	if (!strcmp(genlistItemClass->item_style, "group_index")) {
		if (!strcmp(part, "elm.text")) {
			snprintf(buf, 1023, "%s %d", "Group Index", listItemEobjs->index);
			return strdup(buf);
		}
	} else if (!strcmp(genlistItemClass->item_style, "type1")) {
		if (!strcmp(part, "elm.text")) {
			snprintf(buf, 1023, "[%d]%s:%s", listItemEobjs->index, part, "Subject");
			return strdup(buf);
		} else if (!strcmp(part, "elm.text.sub")) {
			snprintf(buf, 1023, "%s", "Content text will be written here");
			return strdup(buf);
		} else if (!strcmp(part, "elm.text.sub.end")) {
			int month = 7;
			int date = 14;
			snprintf(buf, 1023, "[%d/%d]", month, date);
			return strdup(buf);
		}
	}

	return NULL;
}

static Evas_Object*
gl_content_get_cb(void *data, Evas_Object *obj, const char *part) {
	list_item_eobjs_s *listItemEobjs = data;
	const Elm_Genlist_Item_Class *itc = elm_genlist_item_item_class_get(
			listItemEobjs->mItem);
	Evas_Object *content = NULL;

	if (!strcmp(itc->item_style, "type1")) {
		if (!strcmp(part, "elm.swallow.icon.1")) {
			content = elm_layout_add(obj);
			elm_layout_theme_set(content, "layout", "list/A/right.icon",
					"default");
			Evas_Object *img = create_favorite_icon(content);
			elm_layout_content_set(content, "elm.swallow.content", img);
		}
	}

	return content;
}

static void gl_loaded_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
		void *event_info EINA_UNUSED) {

}

static void gl_realized_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
		void *event_info) {
	/* If you need, you can get item's data. */
	/*
	 Elm_Object_Item *objectItem = event_info;
	 list_item_eobjs_s *listItemEobjs = elm_object_item_data_get(objectItem);
	 */
}

static void gl_longpressed_cb(void *data EINA_UNUSED,
		Evas_Object *obj EINA_UNUSED, void *event_info) {
	/* If you need, you can get item's data. */
	/*
	 Elm_Object_Item *objectItem = event_info;
	 list_item_eobjs_s *listItemEobjs = elm_object_item_data_get(objectItem);
	 */
}

static void gl_selected_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
		void *event_info) {
	Elm_Object_Item *objectItem = event_info;
	list_item_eobjs_s *listItemEobjs = elm_object_item_data_get(objectItem);

	/* Remove highlight from item */
	elm_genlist_item_selected_set(listItemEobjs->mItem, EINA_FALSE);

	app_eobjs_s *ad = listItemEobjs->mAppEobjs;
	proc_info_eobjs_s *cd = calloc(1, sizeof(proc_info_eobjs_s));
	cd->mAppEobjs = ad;
	Evas_Object *btn, *scroller, *main_box, *to, *subject, *content;
	Elm_Object_Item *nf_it;

	scroller = create_scroller(ad->mNaviFrame);

	/* append box */
	main_box = create_box(scroller);
	evas_object_size_hint_weight_set(main_box, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(main_box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_padding_set(main_box, 0, 30);
	elm_object_content_set(scroller, main_box);

	/* to */
	to = create_entry(main_box, NULL, "To");
	evas_object_size_hint_weight_set(to, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(to, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(main_box, to);
	cd->to = to;

	/* subject */
	subject = create_entry(main_box, NULL, "Subject");
	evas_object_size_hint_weight_set(subject, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(subject, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(main_box, subject);
	cd->subject = subject;

	/* content */
	content = create_entry(main_box, NULL, "Content");
	evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(main_box, content);
	cd->content = content;

	nf_it = elm_naviframe_item_push(ad->mNaviFrame, "Process # (Name)", NULL, NULL,
			scroller, NULL);

	/* back button */
	btn = create_button(ad->mNaviFrame, "naviframe/title_done", NULL);
	evas_object_smart_callback_add(btn, "clicked", back_button_clicked_cb, cd);
	elm_object_item_part_content_set(nf_it, "title_left_btn", btn);
}

static Evas_Object*
create_base_layout(Evas_Object *parent_eobj) {
	Evas_Object *layout;
	layout = elm_layout_add(parent_eobj);
	elm_layout_theme_set(layout, "layout", "drawer", "panel");
	evas_object_show(layout);

	return layout;
}

static Evas_Object*
create_panel(Evas_Object *parent_eobj) {
	Evas_Object *panel, *bg;

	panel = elm_panel_add(parent_eobj);
	elm_panel_scrollable_set(panel, EINA_TRUE);
	elm_panel_hidden_set(panel, EINA_TRUE);
	elm_panel_orient_set(panel, ELM_PANEL_ORIENT_LEFT);

	bg = evas_object_rectangle_add(evas_object_evas_get(parent_eobj));
	evas_object_color_set(bg, 0, 0, 0, 0);
	elm_object_part_content_set(parent_eobj, "elm.swallow.bg", bg);
	evas_object_show(bg);

	evas_object_smart_callback_add(panel, "scroll", panel_scroll_cb, bg);
	evas_object_show(panel);

	return panel;
}

static Evas_Object*
create_panel_list(Evas_Object *parent_eobj) {
	Evas_Object *list;

	list = elm_list_add(parent_eobj);
	elm_list_mode_set(list, ELM_LIST_COMPRESS);
	for (int i = 0; i < MENU_NUM; i++) {
		elm_list_item_append(list, gMenuNames[i], NULL, NULL, gMenuCallbacks[i], NULL);
	}

	evas_object_show(list);

	return list;
}

static Evas_Object*
create_genlist(app_eobjs_s *app_eobjs, Evas_Object *parent_eobj) {
	Evas_Object *genlist;
	Elm_Object_Item *git, *it;
	int n_items = NUM_OF_ITEMS;
	int index;

	/* Create item class */
	Elm_Genlist_Item_Class *gitc = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

	gitc->item_style = "group_index";
	gitc->func.text_get = gl_text_get_cb;
	gitc->func.content_get = gl_content_get_cb;
	gitc->func.del = gl_del_cb;

	itc->item_style = "type1";
	itc->func.text_get = gl_text_get_cb;
	itc->func.content_get = gl_content_get_cb;
	itc->func.del = gl_del_cb;

	genlist = elm_genlist_add(parent_eobj);

	elm_object_style_set(genlist, "handler");
	elm_scroller_single_direction_set(genlist,
			ELM_SCROLLER_SINGLE_DIRECTION_HARD);

	/* HOMOGENEOUS MODE
	 If item height is same when each style name is same, Use homogeneous mode. */
	elm_genlist_homogeneous_set(genlist, EINA_TRUE);

	/* COMPRESS MODE
	 For the Mobile view, because it has full window, compress mode should be used. */
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);

	/* Add Smart Callback */
	evas_object_smart_callback_add(genlist, "realized", gl_realized_cb, NULL);
	evas_object_smart_callback_add(genlist, "loaded", gl_loaded_cb, NULL);
	evas_object_smart_callback_add(genlist, "longpressed", gl_longpressed_cb,
	NULL);

	int group_count = 0;
	for (index = 0; index < n_items; index++) {
		list_item_eobjs_s *id = calloc(sizeof(list_item_eobjs_s), 1);
		id->index = index;
		id->mAppEobjs = app_eobjs;
		if (index % 10 == 0) {
			group_count++;
			id->index = group_count;
			git = elm_genlist_item_append(genlist, /* genlist object */
			gitc, /* item class */
			id, /* item class user data */
			NULL, ELM_GENLIST_ITEM_GROUP, /* item type */
			gl_selected_cb, /* select smart callback */
			id); /* smart callback user data */
			id->mItem = git;
		} else {
			it = elm_genlist_item_append(genlist, /* genlist object */
			itc, /* item class */
			id, /* item class user data */
			git, /* parent item */
			ELM_GENLIST_ITEM_NONE, /* item type */
			gl_selected_cb, /* select smart callback */
			id); /* smart callback user data */
			id->mItem = it;
		}
	}
	elm_genlist_item_class_free(gitc);
	elm_genlist_item_class_free(itc);
	evas_object_show(genlist);

	return genlist;
}

static Evas_Object*
create_main_view(app_eobjs_s *app_eobjs) {
	Evas_Object *layout;
	/* Drawer layout */
	layout = create_base_layout(app_eobjs->mConformant);

	/* Panel */
	app_eobjs->mPanel = create_panel(layout);
	elm_object_part_content_set(layout, "elm.swallow.left", app_eobjs->mPanel);

	/* Panel list */
	app_eobjs->mPanelList = create_panel_list(app_eobjs->mPanel);
	evas_object_size_hint_weight_set(app_eobjs->mPanelList, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(app_eobjs->mPanelList, EVAS_HINT_FILL,
	EVAS_HINT_FILL);
	evas_object_smart_callback_add(app_eobjs->mPanelList, "selected", list_clicked_cb,
			app_eobjs);
	eext_object_event_callback_add(app_eobjs->mPanelList, EEXT_CALLBACK_BACK,
			list_back_cb, app_eobjs->mPanel);
	elm_object_content_set(app_eobjs->mPanel, app_eobjs->mPanelList);

	return layout;
}

static void create_base_gui(app_eobjs_s *app_eobjs) {
	Evas_Object *leftBtn, *background;
	Elm_Object_Item *naviFrameItem;

	/* Window */
	app_eobjs->mWindow = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(app_eobjs->mWindow, EINA_TRUE);
	elm_win_autodel_set(app_eobjs->mWindow, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(app_eobjs->mWindow)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(app_eobjs->mWindow,
				(const int *) (&rots), 4);
	}

	evas_object_smart_callback_add(app_eobjs->mWindow, "delete,request",
			win_delete_request_cb,
			NULL);

	/* Conformant */
	app_eobjs->mConformant = elm_conformant_add(app_eobjs->mWindow);
	evas_object_size_hint_weight_set(app_eobjs->mConformant, EVAS_HINT_EXPAND,
	EVAS_HINT_EXPAND);
	elm_win_indicator_mode_set(app_eobjs->mWindow, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(app_eobjs->mWindow, ELM_WIN_INDICATOR_OPAQUE);
	elm_win_resize_object_add(app_eobjs->mWindow, app_eobjs->mConformant);
	evas_object_show(app_eobjs->mConformant);

	/* Indicator BG */
	background = elm_bg_add(app_eobjs->mConformant);
	elm_object_style_set(background, "indicator/headerbg");
	elm_object_part_content_set(app_eobjs->mConformant, "elm.swallow.indicator_bg", background);
	evas_object_show(background);

	/* Main View */
	app_eobjs->mBaseLayout = create_main_view(app_eobjs);
	elm_object_content_set(app_eobjs->mConformant, app_eobjs->mBaseLayout);

	/* Naviframe */
	app_eobjs->mNaviFrame = elm_naviframe_add(app_eobjs->mBaseLayout);
	eext_object_event_callback_add(app_eobjs->mNaviFrame, EEXT_CALLBACK_BACK,
			eext_naviframe_back_cb, app_eobjs);
	elm_object_part_content_set(app_eobjs->mBaseLayout, "elm.swallow.content", app_eobjs->mNaviFrame);
	evas_object_show(app_eobjs->mNaviFrame);

	/* Genlist */
	app_eobjs->mGenList = create_genlist(app_eobjs, app_eobjs->mBaseLayout);

	naviFrameItem = elm_naviframe_item_push(app_eobjs->mNaviFrame, "Total Process List", NULL, NULL,
			app_eobjs->mGenList, NULL);
	elm_naviframe_item_pop_cb_set(naviFrameItem, nf_it_pop_cb, app_eobjs);

	/* left panel toggle button */
	leftBtn = create_button(app_eobjs->mNaviFrame, "naviframe/drawers", NULL);
	evas_object_smart_callback_add(leftBtn, "clicked",
			left_panel_button_clicked_cb, app_eobjs);
	elm_object_item_part_content_set(naviFrameItem, "title_left_btn", leftBtn);

	/* Show window after base gui is set up */
	evas_object_show(app_eobjs->mWindow);
}

/* Application lifecycle event callbacks START */
static bool app_create(void *data) {
	/* Hook to take necessary actions before main event loop starts
	 Initialize UI resources and application's data
	 If this function returns true, the main loop of application starts
	 If this function returns false, the application is terminated */
	app_eobjs_s *app_eobjs = data;

	create_base_gui(app_eobjs);

	return true;
}

static void app_control(app_control_h app_control, void *data) {
	/* Handle the launch request. */
}

static void app_pause(void *data) {
	/* Take necessary actions when application becomes invisible. */
}

static void app_resume(void *data) {
	/* Take necessary actions when application becomes visible. */
}

static void app_terminate(void *data) {
	/* Release all resources. */
}
/* Application lifecycle event callbacks END */

/* Application system event callbacks START */
static void ui_app_lang_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE,
			&locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void ui_app_orient_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void ui_app_region_changed(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void ui_app_low_battery(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_BATTERY*/
}

static void ui_app_low_memory(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_MEMORY*/
}
/* Application system event callbacks END */

/* Main function */
int main(int argc, char *argv[]) {
	app_eobjs_s app_eobjs = { 0, };
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = { 0, };
	app_event_handler_h handlers[5] = { NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY],
			APP_EVENT_LOW_BATTERY, ui_app_low_battery, &app_eobjs);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY],
			APP_EVENT_LOW_MEMORY, ui_app_low_memory, &app_eobjs);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED],
			APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &app_eobjs);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
			APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &app_eobjs);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
			APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &app_eobjs);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &app_eobjs);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d",
				ret);

	return ret;
}