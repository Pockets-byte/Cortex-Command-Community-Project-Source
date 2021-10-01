#include "UInputMan.h"
#include "SceneMan.h"
#include "ActivityMan.h"
#include "FrameMan.h"
#include "ConsoleMan.h"
#include "PresetMan.h"
#include "PerformanceMan.h"
#include "GUIInput.h"
#include "Icon.h"
#include "GameActivity.h"

#include "System/SDLHelper.h"
#include "System/System.h"
#include <SDL2/SDL_keyboard.h>

namespace RTE {

	GUIInput *UInputMan::s_InputClass = nullptr;
#if 0
	char *UInputMan::s_PrevKeyStates = new char[KEY_MAX];
	char *UInputMan::s_ChangedKeyStates = new char[KEY_MAX];

	bool UInputMan::s_CurrentMouseButtonStates[MouseButtons::MAX_MOUSE_BUTTONS];
	bool UInputMan::s_PrevMouseButtonStates[MouseButtons::MAX_MOUSE_BUTTONS];
	bool UInputMan::s_ChangedMouseButtonStates[MouseButtons::MAX_MOUSE_BUTTONS];

	JOYSTICK_INFO UInputMan::s_PrevJoystickStates[Players::MaxPlayerCount];
	JOYSTICK_INFO UInputMan::s_ChangedJoystickStates[Players::MaxPlayerCount];
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::Clear() {
		m_OverrideInput = false;
		m_RawMouseMovement.Reset();
		m_AnalogMouseData.Reset();
		m_MouseSensitivity = 0.6F;
		m_MouseWheelChange = 0;
		m_TrapMousePos = false;
		m_MouseTrapRadius = 350;
		m_LastDeviceWhichControlledGUICursor = InputDevice::DEVICE_KEYB_ONLY;
		m_DisableKeyboard = false;
		m_DisableMouseMoving = false;
		m_PrepareToEnableMouseMoving = false;

		std::fill(std::begin(m_DeviceIcons), std::end(m_DeviceIcons), nullptr);

		m_MouseButtonState.clear();
		m_KeyStates.clear();
		m_ControllerAxisState.clear();
		m_ControllerButtonState.clear();

		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; ++player) {
			m_ControlScheme[player].Reset();

#ifdef NETWORK_ENABLED
			for (int inputState = InputState::Held; inputState < InputState::InputStateCount; inputState++) {
				for (int element = InputElements::INPUT_L_UP; element < InputElements::INPUT_COUNT; element++) {
					m_NetworkInputElementState[player][element][inputState] = false;
				}
				for (int mouseButton = MouseButtons::MOUSE_LEFT; mouseButton < MouseButtons::MAX_MOUSE_BUTTONS; mouseButton++) {
					m_NetworkMouseButtonState[player][mouseButton][inputState] = false;
				}
			}
			m_NetworkAccumulatedRawMouseMovement[player].Reset();
			m_NetworkAnalogMoveData[player].Reset();
			m_NetworkMouseWheelState[player] = 0;
#endif
			m_TrapMousePosPerPlayer[player] = false;
		}
#ifdef NETWORK_ENABLED
		for (int inputState = InputState::Pressed; inputState < InputState::InputStateCount; inputState++) {
			for (int element = InputElements::INPUT_L_UP; element < InputElements::INPUT_COUNT; element++) {
				m_NetworkAccumulatedElementState[element][inputState] = false;
			}
		}
#endif

		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; ++player){
			m_ControlScheme.at(player).Reset();
			m_ControlScheme.at(player).ResetToPlayerDefaults(static_cast<Players>(player));
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UInputMan::Initialize() {
		// TODO: Initialize Controllers
		// SDL_SetRelativeMouseMode(SDL_TRUE);
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::LoadDeviceIcons() {
		m_DeviceIcons[InputDevice::DEVICE_KEYB_ONLY] = dynamic_cast<const Icon *>(g_PresetMan.GetEntityPreset("Icon", "Device Keyboard"));
		m_DeviceIcons[InputDevice::DEVICE_MOUSE_KEYB] = dynamic_cast<const Icon *>(g_PresetMan.GetEntityPreset("Icon", "Device Mouse"));

		for (int gamepad = InputDevice::DEVICE_GAMEPAD_1; gamepad < InputDevice::DEVICE_COUNT; gamepad++) {
			std::string gamepadNum = std::to_string(gamepad - 1);
			m_DeviceIcons[gamepad] = dynamic_cast<const Icon *>(g_PresetMan.GetEntityPreset("Icon", "Device Gamepad " + gamepadNum));
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::SetInputClass(GUIInput *inputClass) const { s_InputClass = inputClass; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector UInputMan::AnalogMoveValues(int whichPlayer) {
		Vector moveValues(0, 0);
		return moveValues;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector UInputMan::AnalogAimValues(int whichPlayer) {
		InputDevice device = m_ControlScheme[whichPlayer].GetDevice();
		return Vector(0, 0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector UInputMan::GetMenuDirectional() {
		Vector allInput(0, 0);
		return allInput;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyKeyOrJoyInput() const {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyPress() const {
		bool pressed = false;

		if (!pressed) {
			pressed = AnyKeyPress();
		}
		if (!pressed) {
			pressed = AnyMouseButtonPress();
		}
		if (!pressed) {
			pressed = AnyJoyPress();
		}

		return pressed;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyStartPress(bool includeSpacebar) {
		if (KeyPressed(SDLK_ESCAPE) || (includeSpacebar && KeyPressed(SDLK_SPACE))) {
			return true;
		}
		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; ++player) {
			if (ElementPressed(player, InputElements::INPUT_START)) {
				return true;
			}
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyBackPress() {
		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; ++player) {
			if (ElementPressed(player, InputElements::INPUT_BACK)) {
				return true;
			}
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyKeyPress() const {
		for (auto key: m_KeyStates) {
			if (key.second)
				return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UInputMan::MouseUsedByPlayer() const {
		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; player++) {
			if (m_ControlScheme[player].GetDevice() == InputDevice::DEVICE_MOUSE_KEYB) {
				return player;
			}
		}
		return Players::NoPlayer;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::DisableMouseMoving(bool disable) {
		if (disable) {
			m_DisableMouseMoving = true;
			m_PrepareToEnableMouseMoving = false;
		} else {
			m_PrepareToEnableMouseMoving = true;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector UInputMan::GetMouseMovement(int whichPlayer) const {
		if (IsInMultiplayerMode() && whichPlayer >= Players::PlayerOne && whichPlayer < Players::MaxPlayerCount) {
			return m_NetworkAccumulatedRawMouseMovement[whichPlayer];
		}
		if (whichPlayer == Players::NoPlayer || m_ControlScheme[whichPlayer].GetDevice() == InputDevice::DEVICE_MOUSE_KEYB) {
			return m_RawMouseMovement;
		}
		return Vector(0, 0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::SetMousePos(Vector &newPos, int whichPlayer) const {
		// Only mess with the mouse if the original mouse position is not above the screen and may be grabbing the title bar of the game window
		if (!m_DisableMouseMoving && !m_TrapMousePos && (whichPlayer == Players::NoPlayer || m_ControlScheme[whichPlayer].GetDevice() == InputDevice::DEVICE_MOUSE_KEYB)) {
			SDL_WarpMouseInWindow(g_FrameMan.GetWindow(), newPos.GetFloorIntX(), newPos.GetFloorIntY());
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyMouseButtonPress() const {
		for (int button = MouseButtons::MOUSE_LEFT; button < MouseButtons::MAX_MOUSE_BUTTONS; ++button) {
			if (MouseButtonPressed(button, -1)) {
				return true;
			}
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::TrapMousePos(bool trap, int whichPlayer) {
		if (whichPlayer == Players::NoPlayer || m_ControlScheme[whichPlayer].GetDevice() == InputDevice::DEVICE_MOUSE_KEYB) {
			m_TrapMousePos = trap;
		}
		m_TrapMousePosPerPlayer[whichPlayer] = trap;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::ForceMouseWithinBox(int x, int y, int width, int height, int whichPlayer) const {
		// Only mess with the mouse if the original mouse position is not above the screen and may be grabbing the title bar of the game window
		if (!m_DisableMouseMoving && !m_TrapMousePos && (whichPlayer == Players::NoPlayer || m_ControlScheme[whichPlayer].GetDevice() == InputDevice::DEVICE_MOUSE_KEYB)) {
			SDL_WarpMouseInWindow(g_FrameMan.GetWindow(), Limit(m_MouseMovement.m_X, x + width * g_FrameMan.GetResMultiplier(), x), Limit(m_MouseMovement.m_Y, y + height * g_FrameMan.GetResMultiplier(), y));
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::ForceMouseWithinPlayerScreen(int whichPlayer) const {
		if (whichPlayer < Players::PlayerOne || whichPlayer >= Players::PlayerFour) {
			return;
		}
		unsigned int screenWidth = g_FrameMan.GetPlayerFrameBufferWidth(whichPlayer);
		unsigned int screenHeight = g_FrameMan.GetPlayerFrameBufferHeight(whichPlayer);

		if (g_FrameMan.GetScreenCount() > 1) {
			switch (whichPlayer) {
				case Players::PlayerOne:
					ForceMouseWithinBox(0, 0, screenWidth, screenHeight, whichPlayer);
					break;
				case Players::PlayerTwo:
					if ((g_FrameMan.GetVSplit() && !g_FrameMan.GetHSplit()) || (g_FrameMan.GetVSplit() && g_FrameMan.GetHSplit())) {
						ForceMouseWithinBox(g_FrameMan.GetResX() / 2, 0, screenWidth, screenHeight, whichPlayer);
					} else {
						ForceMouseWithinBox(0, g_FrameMan.GetResY() / 2, screenWidth, screenHeight, whichPlayer);
					}
					break;
				case Players::PlayerThree:
					ForceMouseWithinBox(0, g_FrameMan.GetResY() / 2, screenWidth, screenHeight, whichPlayer);
					break;
				case Players::PlayerFour:
					ForceMouseWithinBox(g_FrameMan.GetResX() / 2, g_FrameMan.GetResY() / 2, screenWidth, screenHeight, whichPlayer);
					break;
				default:
					RTEAbort("Undefined player value passed in. See Players enumeration for defined values.")
			}
		} else {
			ForceMouseWithinBox(0, 0, screenWidth, screenHeight, whichPlayer);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UInputMan::WhichJoyButtonHeld(int whichJoy) const {
		return JoyButtons::JOY_NONE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UInputMan::WhichJoyButtonPressed(int whichJoy) const {
		return JoyButtons::JOY_NONE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float UInputMan::AnalogAxisValue(int whichJoy, int whichStick, int whichAxis) const {
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyJoyInput(bool checkForPresses) const {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::AnyJoyButtonPress(int whichJoy) const {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector UInputMan::GetNetworkAccumulatedRawMouseMovement(int player) {
#ifdef NEWTWORK_ENABLED
		Vector accumulatedMovement = m_NetworkAccumulatedRawMouseMovement[player];
		m_NetworkAccumulatedRawMouseMovement[player].Reset();
#endif
		return Vector(0, 0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::ClearNetworkAccumulatedStates() {
#ifdef NETWORK_ENABLED
		for (int inputState = InputState::Pressed; inputState < InputState::InputStateCount; inputState++) {
			for (int element = InputElements::INPUT_L_UP; element < InputElements::INPUT_COUNT; element++) {
				m_NetworkAccumulatedElementState[element][inputState] = false;
			}
		}
#endif
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::GetInputElementState(int whichPlayer, int whichElement, InputState whichState) {
		if (IsInMultiplayerMode() && whichPlayer >= Players::PlayerOne && whichPlayer < Players::MaxPlayerCount) {
			return m_TrapMousePosPerPlayer[whichPlayer] ? m_NetworkInputElementState[whichPlayer][whichElement][whichState] : false;
		}
		bool elementState = false;
		InputDevice device = m_ControlScheme[whichPlayer].GetDevice();
		const InputMapping *element = &(m_ControlScheme[whichPlayer].GetInputMappings()->at(whichElement));

		if (!elementState && (device == InputDevice::DEVICE_KEYB_ONLY || (device == InputDevice::DEVICE_MOUSE_KEYB && !(whichElement == InputElements::INPUT_AIM_UP || whichElement == InputElements::INPUT_AIM_DOWN)))) {
			elementState = GetKeyboardButtonState(element->GetKey(), whichState);
		}
		if (!elementState && device == InputDevice::DEVICE_MOUSE_KEYB && m_TrapMousePos) {
			elementState = GetMouseButtonState(whichPlayer, element->GetMouseButton(), whichState);
		}

		if (!elementState && device >= InputDevice::DEVICE_GAMEPAD_1) {
			int whichJoy = GetJoystickIndex(device);
			elementState = GetJoystickButtonState(whichJoy, element->GetJoyButton(), whichState);
			if (!elementState && element->JoyDirMapped()) {
				elementState = GetJoystickDirectionState(whichJoy, element->GetStick(), element->GetAxis(), element->GetDirection(), whichState);
			}
		}
		return elementState;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::GetMenuButtonState(int whichButton, InputState whichState) {
		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; ++player) {
			bool buttonState = false;
			InputDevice device = m_ControlScheme[player].GetDevice();
			if (!buttonState && whichButton >= MenuCursorButtons::MENU_PRIMARY) {
				buttonState = GetInputElementState(player, InputElements::INPUT_FIRE, whichState) || GetMouseButtonState(player, MouseButtons::MOUSE_LEFT, whichState);
			}
			if (!buttonState && whichButton >= MenuCursorButtons::MENU_SECONDARY) {
				buttonState = GetInputElementState(player, InputElements::INPUT_PIEMENU, whichState) || GetMouseButtonState(player, MouseButtons::MOUSE_RIGHT, whichState);
			}
			if (buttonState) {
				m_LastDeviceWhichControlledGUICursor = device;
				return buttonState;
			}
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::FlagAltState() const { return m_ModState & KMOD_ALT; }

	bool UInputMan::FlagCtrlState() const { return m_ModState & KMOD_CTRL; }
	bool UInputMan::FlagShiftState() const { return m_ModState & KMOD_SHIFT; }

	bool UInputMan::KeyPressedScancode(SDL_Scancode keyToTest) const { return KeyPressed(SDL_GetKeyFromScancode(keyToTest)); }

	bool UInputMan::GetKeyboardButtonState(SDL_Keycode whichKey, InputState whichState) const {
		if (m_DisableKeyboard) {
			return false;
		}

		auto key = m_KeyStates.find(whichKey);

		if ((key != m_KeyStates.end()) && (key->second == whichState))
			return true;

		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::GetMouseButtonState(int whichPlayer, int whichButton, InputState whichState) const {
		if (whichButton < MouseButtons::MOUSE_LEFT || whichButton >= MouseButtons::MAX_MOUSE_BUTTONS) {
			return false;
		}

		auto button = m_MouseButtonState.find(whichButton);

		if ((button != m_MouseButtonState.end()) && button->second == whichState) {
			return true;
		}

		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::GetJoystickButtonState(int whichJoy, int whichButton, InputState whichState) const {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool UInputMan::GetJoystickDirectionState(int whichJoy, int whichStick, int whichAxis, int whichDir, InputState whichState) const {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UInputMan::Update() {
		m_LastDeviceWhichControlledGUICursor = InputDevice::DEVICE_KEYB_ONLY;
		UpdateKeyStates();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				System::SetQuit();
			else if (e.type == SDL_KEYDOWN) {
				PressKey(e.key.keysym.sym);
				m_ModState = e.key.keysym.mod;
			} else if (e.type == SDL_KEYUP) {
				ReleaseKey(e.key.keysym.sym);
				m_ModState = e.key.keysym.mod;
			} else if (e.type == SDL_MOUSEMOTION){
				m_MouseMovement.m_X  = e.motion.xrel;
				m_MouseMovement.m_Y = e.motion.yrel;
				m_MousePos.m_X = e.motion.x;
				m_MousePos.m_Y = e.motion.y;
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				PressMouseButton(e.button.button);
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				ReleaseMouseButton(e.button.button);
			}
		}
		HandleSpecialInput();
		return 0;
	}


	void UInputMan::PressKey(SDL_Keycode key) {
		if(m_KeyStates.find(key) != m_KeyStates.end() && m_KeyStates[key] == InputState::Held)
			return;
		m_KeyStates[key] = InputState::Pressed;
	}

	void UInputMan::ReleaseKey(SDL_Keycode key) {
		m_KeyStates[key] = InputState::Released;
	}

	void UInputMan::UpdateKeyStates() {
		for(auto key = m_KeyStates.begin(); key != m_KeyStates.end(); key++){
			if(key->second == InputState::Pressed)
				key->second = InputState::Held;
			else if(key->second  == InputState::Released)
				key->second = InputState::None;
		}

		for(auto button = m_MouseButtonState.begin(); button != m_MouseButtonState.end(); button++){
			if(button->second == InputState::Pressed)
				button->second = InputState::Held;
			else if (button->second == InputState::Released)
				button->second = InputState::None;
		}
	}

	void UInputMan::PressMouseButton(int button) {
		if (m_MouseButtonState.find(button) != m_MouseButtonState.end() && m_MouseButtonState[button] == InputState::Held)
			return;
		m_MouseButtonState[button] = InputState::Pressed;
	}

	void UInputMan::ReleaseMouseButton(int button) {
		m_MouseButtonState[button] = InputState::Released;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::HandleSpecialInput() {
		// If we launched into editor directly, skip the logic and quit quickly.
		if (KeyPressed(SDLK_ESCAPE) && g_ActivityMan.IsSetToLaunchIntoEditor()) {
			System::SetQuit();
			return;
		}

		if (g_ActivityMan.IsInActivity()) {
			const GameActivity *gameActivity = dynamic_cast<GameActivity *>(g_ActivityMan.GetActivity());
			if (AnyStartPress(false) && (!gameActivity || !gameActivity->IsBuyGUIVisible(-1))) {
				g_ActivityMan.PauseActivity();
				return;
			}
			// Ctrl+R or Back button for controllers to reset activity.
			if (!g_ActivityMan.ActivitySetToRestart()) {
				g_ActivityMan.SetRestartActivity((FlagCtrlState() && KeyPressed(SDLK_r)) || AnyBackPress());
			}
			if (g_ActivityMan.ActivitySetToRestart()) {
				return;
			}
		}

		if (FlagCtrlState() && !FlagAltState()) {
			// Ctrl+S to save continuous ScreenDumps
			if (KeyHeld(SDLK_s)) {
				g_FrameMan.SaveScreenToPNG("ScreenDump");
				// Ctrl+W to save a WorldDump
			} else if (KeyPressed(SDLK_w)) {
				g_FrameMan.SaveWorldToPNG("WorldDump");
				// Ctrl+M to cycle draw modes
			} else if (KeyPressed(SDLK_m)) {
				g_SceneMan.SetLayerDrawMode((g_SceneMan.GetLayerDrawMode() + 1) % 3);
				// Ctrl+P to toggle performance stats
			} else if (KeyPressed(SDLK_p)) {
				g_PerformanceMan.ShowPerformanceStats(!g_PerformanceMan.IsShowingPerformanceStats());
				// Ctrl+O to toggle one sim update per frame
			} else if (KeyPressed(SDLK_o)) {
				g_TimerMan.SetOneSimUpdatePerFrame(!g_TimerMan.IsOneSimUpdatePerFrame());
			}
		} else if (!FlagCtrlState() && FlagAltState()) {
			// Alt+Enter to switch resolution multiplier
			if (KeyPressed(SDLK_RETURN)) {
				g_FrameMan.SwitchResolutionMultiplier(g_FrameMan.IsFullscreen() ? 1 : 2);
				g_FrameMan.SetFullscreen(!g_FrameMan.IsFullscreen());
			// Alt+W to save ScenePreviewDump (miniature WorldDump)
			} else if (KeyPressed(SDLK_w)) {
				// g_FrameMan.SaveWorldPreviewToPNG("ScenePreviewDump");
			}
		} else {
			// PrntScren to save a single ScreenDump
			if (KeyPressed(SDLK_PRINTSCREEN)) {
				g_FrameMan.SaveScreenToPNG("ScreenDump");
			} else if (KeyPressed(SDLK_F1)) {
				g_ConsoleMan.ShowShortcuts();
			} else if (KeyPressed(SDLK_F2)) {
				g_PresetMan.ReloadAllScripts();
				g_ConsoleMan.PrintString("SYSTEM: Scripts reloaded!");
			} else if (KeyPressed(SDLK_F3)) {
				g_ConsoleMan.SaveAllText("Console.dump.log");
			} else if (KeyPressed(SDLK_F4)) {
				g_ConsoleMan.SaveInputLog("Console.input.log");
			} else if (KeyPressed(SDLK_F5)) {
				g_ConsoleMan.ClearLog();
			}

			if (g_PerformanceMan.IsShowingPerformanceStats()) {
				// Manipulate time scaling
				if (KeyHeld(SDLK_2)) {
					g_TimerMan.SetTimeScale(g_TimerMan.GetTimeScale() + 0.01F);
				}
				if (KeyHeld(SDLK_1) && g_TimerMan.GetTimeScale() - 0.01F > 0.001F) {
					g_TimerMan.SetTimeScale(g_TimerMan.GetTimeScale() - 0.01F);
				}

				// Manipulate real to sim cap
				if (KeyHeld(SDLK_4)) {
					g_TimerMan.SetRealToSimCap(g_TimerMan.GetRealToSimCap() + 0.001F);
				}
				if (KeyHeld(SDLK_3) && g_TimerMan.GetRealToSimCap() > 0) {
					g_TimerMan.SetRealToSimCap(g_TimerMan.GetRealToSimCap() - 0.001F);
				}

				// Manipulate DeltaTime
				if (KeyHeld(SDLK_6)) {
					g_TimerMan.SetDeltaTimeSecs(g_TimerMan.GetDeltaTimeSecs() + 0.001F);
				}
				if (KeyHeld(SDLK_5) && g_TimerMan.GetDeltaTimeSecs() > 0) {
					g_TimerMan.SetDeltaTimeSecs(g_TimerMan.GetDeltaTimeSecs() - 0.001F);
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NETWORK_ENABLED
	void UInputMan::UpdateNetworkMouseMovement() {
		for (int player = Players::PlayerOne; player < Players::MaxPlayerCount; player++) {
			if (!m_NetworkAccumulatedRawMouseMovement[player].IsZero()) {
				// TODO: Figure out why we're multiplying by 3 here. Possibly related to mouse sensitivity.
				m_NetworkAnalogMoveData[player].m_X += m_NetworkAccumulatedRawMouseMovement[player].m_X * 3;
				m_NetworkAnalogMoveData[player].m_Y += m_NetworkAccumulatedRawMouseMovement[player].m_Y * 3;
				m_NetworkAnalogMoveData[player].CapMagnitude(m_MouseTrapRadius);
			}
			m_NetworkAccumulatedRawMouseMovement[player].Reset();

			// Clear mouse events and inputs as they should've been already processed during by recipients.
			// This is important so mouse readings are correct, e.g. to ensure events don't trigger multiple times on a single press.
			for (int inputState = InputState::Pressed; inputState < InputState::InputStateCount; inputState++) {
				for (int element = InputElements::INPUT_L_UP; element < InputElements::INPUT_COUNT; element++) {
					m_NetworkInputElementState[player][element][inputState] = false;
				}
				for (int mouseButton = MouseButtons::MOUSE_LEFT; mouseButton < MouseButtons::MAX_MOUSE_BUTTONS; mouseButton++) {
					m_NetworkMouseButtonState[player][mouseButton][inputState] = false;
				}
			}

			// Reset mouse wheel state to stop over-wheeling
			m_NetworkMouseWheelState[player] = 0;
		}
	}
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void UInputMan::UpdateMouseInput() {
		// Detect and store mouse movement input, translated to analog stick emulation
		int mousePlayer = MouseUsedByPlayer();
		if (mousePlayer != Players::NoPlayer) {
			// TODO: Figure out why we're multiplying by 3 here. Possibly related to mouse sensitivity.
			m_AnalogMouseData = m_MouseMovement;
			m_AnalogMouseData.CapMagnitude(m_MouseTrapRadius);

			// Only mess with the mouse pos if the original mouse position is not above the screen and may be grabbing the title bar of the game window
			if (!m_DisableMouseMoving && !IsInMultiplayerMode()) {
				if (m_TrapMousePos) {
					// Trap the (invisible) mouse cursor in the middle of the screen, so it doesn't fly out in windowed mode and some other window gets clicked
					SDL_WarpMouseInWindow(g_FrameMan.GetWindow(), g_FrameMan.GetResX() / 2, g_FrameMan.GetResY() / 2);
				} else if (g_ActivityMan.IsInActivity()) {
					// The mouse cursor is visible and can move about the screen/window, but it should still be contained within the mouse player's part of the window
					ForceMouseWithinPlayerScreen(mousePlayer);
				}
			}

			// Enable the mouse cursor positioning again after having been disabled. Only do this when the mouse is within the drawing area so it
			// won't cause the whole window to move if the user clicks the title bar and unintentionally drags it due to programmatic positioning.
			int mousePosX = m_MouseMovement.m_X / g_FrameMan.GetResMultiplier();
			int mousePosY = m_MouseMovement.m_Y / g_FrameMan.GetResMultiplier();
			if (m_DisableMouseMoving && m_PrepareToEnableMouseMoving && (mousePosX >= 0 && mousePosX < g_FrameMan.GetResX() && mousePosY >= 0 && mousePosY < g_FrameMan.GetResY())) {
				m_DisableMouseMoving = m_PrepareToEnableMouseMoving = false;
			}
		}
		if (mousePlayer != Players::NoPlayer || g_ActivityMan.IsInActivity() == false) {
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RTE
