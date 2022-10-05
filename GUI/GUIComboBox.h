#ifndef _GUICOMBOBOX_
#define _GUICOMBOBOX_

#include "GUITextPanel.h"
#include "GUIListPanel.h"

namespace RTE {

class GUIComboBoxButton;

/// <summary>
/// A ComboBox control class.
/// </summary>
class GUIComboBox : public GUIControl, public GUIPanel {

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:

    // Notifications
    enum {
        Dropped,        // When listpanel has dropped
        Closed,            // When listpanel has closed
    } Notifications;

    // Combo Style
    enum {
        DropDown,
        DropDownList,
    } DropDownStyles;


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUIComboBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUIComboBox object in
//                  system memory.
// Arguments:       GUIManager, GUIControlManager.

    GUIComboBox(GUIManager *Manager, GUIControlManager *ControlManager);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Name, Position.

    void Create(const std::string &Name, int X, int Y, int Width = -1, int Height = -1) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Properties.

    void Create(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been destroyed.
// Arguments:       None.

    void Destroy() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Activate
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control is activated and ready for use.
// Arguments:       None.

    void Activate() override;


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
// Method:          GetPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the panel of the control.
// Arguments:       None.
// Returns:         0 if the control does not have a panel, otherwise the topmost panel.

    GUIPanel * GetPanel() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetListPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the ListPanel component of the control.
// Arguments:       None.
// Returns:         The ListPanel component of this ComboBox.

    GUIListPanel * GetListPanel() { return m_ListPanel; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetControlID
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns a string representing the control's ID
// Arguments:       None.

    static std::string GetControlID()    { return "COMBOBOX"; };


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ReceiveSignal
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when receiving a signal.
// Arguments:       Signal source, Signal code, Signal data.

    void ReceiveSignal(GUIPanel *Source, int Code, int Data) override;

    
//////////////////////////////////////////////////////////////////////////////////////////
// Method:          BeginUpdate
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Locks the control from updating every time a new item is added.
// Arguments:       None.

    void BeginUpdate();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          EndUpdate
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     UnLocks the control from updating every time a new item is added.
//                  Will automatically update the control.
// Arguments:       None.

    void EndUpdate();


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
// Method:          GetControlRect
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the rectangle of the control.
// Arguments:       Position, Size.

    void GetControlRect(int *X, int *Y, int *Width, int *Height) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          AddItem
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Add an item to the list.
// Arguments:       Name, Extra text, bitmap to show in the list, extra entity data

    void AddItem(const std::string &Name, const std::string &ExtraText = "", GUIBitmap *pBitmap = nullptr, const Entity *pEntity = nullptr);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          DeleteItem
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Delete an item from the list.
// Arguments:       Item Index.

    void DeleteItem(int Index);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ClearList
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Clears the list.
// Arguments:       None.

    void ClearList();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetCount
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Get the item count.
// Arguments:       None.

    int GetCount();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetSelectedIndex
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Get the index of the selected item.
// Arguments:       None.

    int GetSelectedIndex();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetOldSelectionIndex
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Get the index of the previously selected item before the selection is
//                  made.
// Arguments:       None.

    int GetOldSelectionIndex() const { return m_OldSelection; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetSelectedIndex
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the index of the selected item.
// Arguments:       None.

    void SetSelectedIndex(int Index);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          RollbackSelection
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Rolls back the selection to the previous selected item.
// Arguments:       None.
// Returns:         Whether the rollback worked and was performed.

    bool RollbackSelection();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetItem
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the Item structure at the index.
// Arguments:       Index.
// Returns:         Pointer to the item structure. 0 if the index was invalid.

    GUIListPanel::Item * GetItem(int Index);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetSelectedItem
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the Item structure at the currently selected index.
// Arguments:       Index.
// Returns:         Pointer to the item structure. 0 if nothing valid is selected.

    GUIListPanel::Item * GetSelectedItem() { return GetItem(GetSelectedIndex()); }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDropHeight
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the drop height of the list.
// Arguments:       Height.

    void SetDropHeight(int Drop);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          StoreProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the control to store the values into properties.
// Arguments:       None.

    void StoreProperties() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDropDownStyle
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the drop down style of the combo box.
// Arguments:       Style.

    void SetDropDownStyle(int Style);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetDropDownStyle
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the drop down style of the combo box.
// Arguments:       None.

    int GetDropDownStyle() const;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetVisible
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the visibility of the control.
// Arguments:       Visible.

	void SetVisible(bool Visible) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetVisible
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the visibility of the control.
// Arguments:       None.

    bool GetVisible() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetEnabled
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the enabled state of the control.
// Arguments:       Enabled.

    void SetEnabled(bool Enabled) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetEnabled
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the enabled state of the control.
// Arguments:       None.

    bool GetEnabled() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetText
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets text (only if style is DropDown).
// Arguments:       None.
// Returns:         Text. Returns empty string is style is not DropDown.

    std::string GetText();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetText
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets text (only if style is DropDown).
// Arguments:       Text.

    void SetText(const std::string &Text);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ApplyProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Applies new properties to the control.
// Arguments:       GUIProperties.

    void ApplyProperties(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          IsDropped
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Shows whether the list is currently dropped down or not.
// Arguments:       None.
// Returns:         Whether this is currently dropped down and showing the list.

    bool IsDropped() { return m_ListPanel->_GetVisible(); }

private:

    GUIBitmap *m_DrawBitmap;
    int m_OldSelection;
    bool m_CreatedList;

    int m_DropHeight;
    int m_DropDownStyle;

    GUITextPanel *m_TextPanel;
    GUIListPanel *m_ListPanel;
    GUIComboBoxButton *m_Button;
};


/// <summary>
/// A ComboBoxButton control class.
/// </summary>
class GUIComboBoxButton : public GUIPanel {

public:

    // Signals
    enum {
        Clicked
    } Signals;


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUIComboBoxButton
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUIComboBoxButton object in
//                  system memory.
// Arguments:       GUIManager.

    explicit GUIComboBoxButton(GUIManager *Manager);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ChangeSkin
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the skin has been changed.
// Arguments:       New skin pointer.

    void ChangeSkin(GUISkin *Skin);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Draw
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws the panel
// Arguments:       Screen class

    void Draw(GUIScreen *Screen) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Create the panel.
// Arguments:       Position, Size.

    void Create(int X, int Y, int Width, int Height);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destroys the button.
// Arguments:       None.

    void Destroy();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes down on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseDown(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes up on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseUp(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetPushed
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the pushed state of the button.
// Arguments:       Pushed.

    void SetPushed(bool Pushed);

private:

    GUIBitmap *m_DrawBitmap;
    bool m_Pushed;
};
};
#endif