#include "GUI.h"
#include "SDLInput.h"

#include "FrameMan.h"
#include "UInputMan.h"

#include "Vector.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>

namespace RTE {
	SDLInput::SDLInput(int whichPlayer, bool keyJoyMouseCursor) :
	    GUIInput(whichPlayer, keyJoyMouseCursor) {
		std::fill(m_KeyboardBuffer, m_KeyboardBuffer + KEYBOARD_BUFFER_SIZE, Released);
		std::fill(m_ScanCodeState, m_ScanCodeState + KEYBOARD_BUFFER_SIZE, Released);
	}

	// TODO: This wraps legacy code; Would need to refactor the whole GUI Lib to fix
	void SDLInput::Update() {

		int numkeys{0};
		const uint8_t *keyboardStatus = SDL_GetKeyboardState(&numkeys);

		for (int key{0}; key < numkeys && key < KEYBOARD_BUFFER_SIZE; key++) {
			if (keyboardStatus[key]) {
				if (m_ScanCodeState[key] == Pushed || m_ScanCodeState[key] == Repeat) {
					m_ScanCodeState[key] = Repeat;
					uint8_t keyname = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(key)); // TODO: srsly do not do it this way, just wrap UInputMan for GUI thatll do
					m_KeyboardBuffer[keyname] = Repeat;
				} else {
					m_ScanCodeState[key] = Pushed;
					uint8_t keyname = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(key));
					m_KeyboardBuffer[keyname] = Pushed;
				}
			} else {
				m_ScanCodeState[key] = Released;
				uint8_t keyname = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(key));
				m_KeyboardBuffer[keyname] = Released;
			}
		}

		keyboardStatus = nullptr;

		m_Modifier = ModNone;

		if (g_UInputMan.FlagAltState())
			m_Modifier |= ModAlt;
		if (g_UInputMan.FlagCtrlState())
			m_Modifier |= ModCtrl;
		if (g_UInputMan.FlagShiftState())
			m_Modifier |= ModShift;

		SDL_GetMouseState(&m_MouseX, &m_MouseY);
		m_LastFrameMouseX = m_MouseX;
		m_LastFrameMouseY = m_MouseY;
		if (!m_KeyJoyMouseCursor) {
			if (g_UInputMan.MouseButtonPressed(SDL_BUTTON_LEFT)) {
				m_MouseButtonsEvents[0] = Pushed;
				m_MouseButtonsStates[0] = Down;
			} else if (g_UInputMan.MouseButtonHeld(SDL_BUTTON_LEFT)) {
				m_MouseButtonsEvents[0] = Repeat;
				m_MouseButtonsStates[0] = Down;
			} else {
				m_MouseButtonsEvents[0] = (m_MouseButtonsStates[0] == Down) ? Released : None;
				m_MouseButtonsStates[0] = Up;
			}
		}

		if (g_UInputMan.MouseButtonPressed(SDL_BUTTON_RIGHT)) {
			std::cout << "Button pushed" << std::endl;
			m_MouseButtonsEvents[1] = Pushed;
			m_MouseButtonsStates[1] = Down;
		} else if (g_UInputMan.MouseButtonHeld(SDL_BUTTON_RIGHT)) {
			std::cout << "Button held" << std::endl;
			m_MouseButtonsEvents[1] = Repeat;
			m_MouseButtonsStates[1] = Down;
		} else {
			std::cout << "Button released" << std::endl;
			m_MouseButtonsEvents[1] = (m_MouseButtonsStates[0] == Down) ? Released : None;
			m_MouseButtonsStates[1] = Up;
		}

		if (g_UInputMan.MouseButtonPressed(SDL_BUTTON_MIDDLE)) {
			m_MouseButtonsEvents[2] = Pushed;
			m_MouseButtonsStates[2] = Down;
		} else if (g_UInputMan.MouseButtonHeld(SDL_BUTTON_MIDDLE)) {
			m_MouseButtonsEvents[2] = Repeat;
			m_MouseButtonsStates[2] = Down;
		} else {
			m_MouseButtonsEvents[2] = (m_MouseButtonsStates[0] == Down) ? Released : None;
			m_MouseButtonsStates[2] = Up;
		}

		if (m_KeyJoyMouseCursor) {

			// Button states/presses, Primary - ACTUALLY make either button work, we don't have use for secondary in menus
			if (g_UInputMan.MenuButtonHeld(UInputMan::MENU_EITHER)) {
				std::cout << "Menu Button held" << std::endl;
				m_MouseButtonsStates[0] = Down;
				m_MouseButtonsEvents[0] = Repeat;
			}
			if (g_UInputMan.MenuButtonPressed(UInputMan::MENU_EITHER)) {
				std::cout << "Menu button pressed" << std::endl;
				m_MouseButtonsStates[0] = Down;
				m_MouseButtonsEvents[0] = Pushed;
			} else if (g_UInputMan.MenuButtonReleased(UInputMan::MENU_EITHER)) {
				// std::cout << "Menu button released" << std::endl;
				m_MouseButtonsStates[0] = Up;
				m_MouseButtonsEvents[0] = Released;
			} else if (m_MouseButtonsEvents[0] == Released) {
				m_MouseButtonsStates[0] = Up;
				m_MouseButtonsEvents[0] = None;
			}
		}
	}

	void ConvertKeyEvent(int sdlKey, int guilibKey, float elapsedS) {
	}
} // namespace RTE
