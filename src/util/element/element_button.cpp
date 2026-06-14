/*************************************************************************
 * This file is part of input-overlay
 * git.vrsal.cc/alex/input-overlay
 * Copyright 2025 univrsal <uni@vrsal.xyz>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "element_button.hpp"

#include "hook/uiohook_helper.hpp"
#include "sources/input_source.hpp"

#include <QJsonArray>

void element_button::load(const QJsonObject &obj)
{
    element_texture::load(obj);
    m_keycode = static_cast<uint16_t>(obj[CFG_KEY_CODE].toInt());
    m_pressed = m_mapping;
    m_pressed.y = m_mapping.y + m_mapping.cy + CFG_INNER_BORDER;
}

void element_keyboard_key::load(const QJsonObject &obj)
{
    element_button::load(obj); // loads m_keycode from "code" as usual

    // if "code" is an array, load all codes into m_keycodes instead
    if (obj[CFG_KEY_CODE].isArray()) {
        m_keycodes.clear();
        for (const auto &v : obj[CFG_KEY_CODE].toArray())
            m_keycodes.push_back(static_cast<uint16_t>(v.toInt()));
    }
}

void element_keyboard_key::draw(gs_effect_t *effect, gs_image_file_t *image, sources::overlay_settings *settings)
{
    bool pressed;
    if (m_keycodes.empty()) {
        pressed = settings->data.keyboard[m_keycode];
    } else {
        pressed = true;
        for (auto code : m_keycodes)
            pressed = pressed && settings->data.keyboard[code];
    }

    if (pressed)
        element_texture::draw(effect, image, &m_pressed);
    else
        element_button::draw(effect, image, nullptr);
}

void element_mouse_button::load(const QJsonObject &obj)
{
    element_button::load(obj);
}

void element_mouse_button::draw(gs_effect_t *effect, gs_image_file_t *image, sources::overlay_settings *settings)
{
    if (settings->data.mouse[m_keycode])
        element_texture::draw(effect, image, &m_pressed);
    else
        element_button::draw(effect, image, nullptr);
}

void element_gamepad_button::draw(gs_effect_t *effect, gs_image_file_t *image, sources::overlay_settings *settings)
{
    if (settings->is_pad_button_pressed(m_keycode))
        element_texture::draw(effect, image, &m_pressed);
    else
        element_button::draw(effect, image, nullptr);
}