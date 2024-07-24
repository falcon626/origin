#pragma once

namespace Key // Recommended To Use
{
	// Key Control Flg
	constinit thread_local auto is_push{ false };

	enum Code : size_t // Usable Key
	{
		Constant = 0x8000,

		Zero     = '0',
		One, Two, Three, Four, Five, Six, Seven, Eight, Nine,

		Num_Zero = VK_NUMPAD0,
		Num_One, Num_Two, Num_Three, Num_Four, Num_Five, Num_Six, Num_Seven, Num_Eight, Num_Nine,

		A        = 'A',
		B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		F1       = VK_F1,
		F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

		L_Click    = VK_LBUTTON,  R_Click   = VK_RBUTTON,  Wheel_Click = VK_MBUTTON, SideL_Click = VK_XBUTTON1, SideR_Click = VK_XBUTTON2,
		Space      = VK_SPACE,    BackSpace = VK_BACK,     Enter       = VK_RETURN,  Esc         = VK_ESCAPE,
		Tab        = VK_TAB,      Delete    = VK_DELETE,
		L_Ctrl     = VK_LCONTROL, R_Ctrl    = VK_RCONTROL, Ctrl        = VK_CONTROL,
		L_Shift    = VK_LSHIFT,   R_Shift   = VK_RSHIFT,   Shift       = VK_SHIFT,
		Up         = VK_UP,       Down      = VK_DOWN,     Left        = VK_LEFT,    Right       = VK_RIGHT,
		UnderScore = VK_OEM_102,  Slash     = VK_OEM_2,
	};

	// Key Function
	inline const auto IsPushing(_In_ const Code& keyCode) noexcept { return (GetAsyncKeyState(keyCode) & Code::Constant) != Def::IntZero; }
	
	inline const auto IsPushing(_In_ const std::initializer_list<Code>& keyCodes) noexcept 
	{ 
		auto isPush{ false };
		for (const auto& keyCode : keyCodes)
			if ((isPush = (GetAsyncKeyState(keyCode) & Code::Constant))) break;
		return   isPush;
	}
	
	// Key Function And Focus Window Check
	inline const auto IsPushingWithFocus(_In_ const Code& keyCode) noexcept
	{ 
		if (KdWindow::Instance().GetWndHandle() != GetForegroundWindow()) return false;
		else return (GetAsyncKeyState(keyCode) & Code::Constant) != Def::IntZero;
	}

	inline const auto IsPushingWithFocus(_In_ const std::initializer_list<Code>& keyCodes) noexcept
	{
		if (KdWindow::Instance().GetWndHandle() != GetForegroundWindow()) return false;
		auto isPush{ false };
		for (const auto& keyCode : keyCodes)
			if ((isPush = (GetAsyncKeyState(keyCode) & Code::Constant))) break;
		return   isPush;
	}
}