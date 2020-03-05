#ifndef _ALLEGROSCREEN_
#define _ALLEGROSCREEN_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            AllegroScreen.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     AllegroScreen class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


struct BITMAP;

namespace RTE
{

class AllegroBitmap;


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           AllegroScreen
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     A screen interface using the CDX Library screen
// Parent(s):       GUIScreen
// Class history:   1/01/2004 AllegroScreen Created.

class AllegroScreen :
    public GUIScreen
{

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     AllegroScreen
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a ScreenInterface object in 
//                  system memory.
// Arguments:       A bitmp that represents the back buffer. Ownership IS NOT transferred!

    AllegroScreen(BITMAP *pBackBuffer);


//////////////////////////////////////////////////////////////////////////////////////////
// Destructor:      ~AllegroScreen
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destructor method used to clean up a AllegroScreen object before deletion
//                  from system memory.
// Arguments:       None.

    virtual ~AllegroScreen() { Destroy(); }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destroy the screen
// Arguments:       None.

    virtual void Destroy(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CreateBitmap
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Creates a bitmap from a file
// Arguments:       Filename

    GUIBitmap *CreateBitmap(const std::string Filename);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CreateBitmap
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Creates an empty bitmap
// Arguments:       Width and Height

    GUIBitmap *CreateBitmap(int Width, int Height);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          DrawBitmap
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws a bitmap onto the back buffer
// Arguments:       Bitmap, destination position, source rectangle

    void DrawBitmap(GUIBitmap *pGUIBitmap, int X, int Y, RECT *pRect);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          DrawBitmapTrans
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws a bitmap onto the back buffer using the colorkey.
// Arguments:       Bitmap, destination position, source rectangle

    void DrawBitmapTrans(GUIBitmap *pGUIBitmap, int X, int Y, RECT *pRect);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetBitmap
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets a bitmap representing the screen.
// Arguments:       None.

    GUIBitmap *GetBitmap(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ConvertColor
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Converts an 8bit palette index to a valid pixel format.
//                  Primarily used for development in windowed mode.
// Arguments:       Color value in any bit depth. Will be converted to the format specified.
//                  An optional target color depth that will determine what format the color
//                  should be converted to. If this is 0, then the current video color depth
//                  will be used as target.

    unsigned long ConvertColor(unsigned long color, int targetDepth = 0);


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable and method declarations

private:


    AllegroBitmap   *m_pBackBitmap;

};


}; // namespace RTE


#endif  //  _CDXSCREENINTERFACE_