#ifndef _GUILABEL_
#define _GUILABEL_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUILabel.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUILabel class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


namespace RTE
{


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUILabel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     A label control class.
// Parent(s):       GUIControl, Panel.
// Class history:   1/22/2004 GUILabel Created.

class GUILabel :
    public GUIControl,
    public GUIPanel
{

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:

    // Label Notifications
    enum {
        Clicked = 0,
    } Notification;


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUILabel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUILabel object in
//                  system memory.
// Arguments:       GUIManager, GUIControlManager.

    GUILabel(GUIManager *Manager, GUIControlManager *ControlManager);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Name, Position.

    void Create(const std::string Name, int X, int Y, int Width = -1, int Height = -1) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Properties.

    void Create(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ChangeSkin
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the skin has been changed.
// Arguments:       New skin pointer.

    void ChangeSkin(GUISkin *Skin) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Draw
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws the panel
// Arguments:       Screen class

    void Draw(GUIScreen *Screen) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:  OnMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes down on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseDown(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:  OnMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes up on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseUp(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the panel of the control.
// Arguments:       None.
// Returns:         0 if the control does not have a panel, otherwise the topmost panel.

    GUIPanel * GetPanel() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetControlID
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns a string representing the control's ID
// Arguments:       None.

    static std::string GetControlID()    { return "LABEL"; };


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Move
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control needs to be moved.
// Arguments:       New position.

    void Move(int X, int Y) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Resize
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control needs to be resized.
// Arguments:       New size.

    void Resize(int Width, int Height) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ResizeHeightToFit
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Resize the height of the label to fit the amount of text it has to
//                  display.
// Arguments:       None.
// Returns:         The new height in pixels.

    int ResizeHeightToFit();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetControlRect
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the rectangle of the control.
// Arguments:       Position, Size.

    void GetControlRect(int *X, int *Y, int *Width, int *Height) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetText
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the text of the label.
// Arguments:       Text.

    void SetText(const std::string Text);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetText
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the text of the label.
// Arguments:       None.

    std::string GetText();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetTextHeight
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Shows how tall the current text is with the current width and font etc.
// Arguments:       None.
// Returns:         The text height, in pixels

    int GetTextHeight();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetHAlignment
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the horizontal alignment of the text of this label.
// Arguments:       Teh desired alignement.

    void SetHAlignment(int HAlignment = GUIFont::Left) { m_HAlignment = HAlignment; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetVAlignment
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the vertical alignment of the text of this label.
// Arguments:       Teh desired alignement.

    void SetVAlignment(int VAlignment = GUIFont::Top) { m_VAlignment = VAlignment; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetHAlignment
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the horizontal alignment of the text of this label.
// Arguments:       None.

    int GetHAlignment() { return m_HAlignment; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetVAlignment
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the vertical alignment of the text of this label.
// Arguments:       Teh desired alignement.

    int GetVAlignment() { return m_VAlignment; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          StoreProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the control to store the values into properties.
// Arguments:       None.

    void StoreProperties() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ApplyProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Applies new properties to the control.
// Arguments:       GUIProperties.

    void ApplyProperties(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable and method declarations

private:
    
    std::string        m_Text;
    int                m_HAlignment;
    int                m_VAlignment;
};


}; // namespace RTE


#endif  //  _GUILABEL_