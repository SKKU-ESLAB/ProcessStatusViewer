/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __processstatusviewer_H__
#define __processstatusviewer_H__

#include <app.h>
#include <system_settings.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dlog.h>

#define NUM_OF_ITEMS 80

#if !defined(PACKAGE)
#define PACKAGE "skku.ospractice.psviewer"
#endif

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "processstatusviewer"

#define ICON_DIR "/opt/usr/apps/skku.ospractice.psviewer/res/images"

/* Menu Configuration START */
#define MENU_NUM 2
void total_process_list_cb(void *data, Evas_Object *obj, void *event_info);
void process_tree_cb(void *data, Evas_Object *obj, void *event_info);

const char gMenuNames[MENU_NUM][50] = { "Total Process List",
		"Process Tree" };

Evas_Smart_Cb gMenuCallbacks[MENU_NUM] = { total_process_list_cb,
		process_tree_cb };
/* Menu Configuration END */

#endif /* __processstatusviewer_H__ */
