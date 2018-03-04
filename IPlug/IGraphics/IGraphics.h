#pragma once

/**
 * @file
 * @copydoc IGraphics
 */

#ifndef NO_IGRAPHICS
#if defined(IGRAPHICS_AGG) + defined(IGRAPHICS_CAIRO) + defined(IGRAPHICS_NANOVG) + defined(IGRAPHICS_LICE) != 1
#error Either NO_IGRAPHICS or one and only one choice of graphics library must be defined!
#endif
#endif

#ifdef IGRAPHICS_FREETYPE
#include "ft2build.h"
#include FT_FREETYPE_H
#endif

#ifdef AAX_API
#include "IPlugAAX_view_interface.h"
#endif

#include "IGraphicsConstants.h"
#include "IGraphicsStructs.h"
#include "IGraphicsUtilites.h"
#include "IPopupMenu.h"

#ifdef OS_MAC
#ifdef FillRect
#undef FillRect
#endif

#ifdef DrawText
#undef DrawText
#endif
#endif

class IDelegate;
class IControl;
class IParam;

/**
 * \defgroup DrawClasses IGraphics::DrawClasses
 * \defgroup PlatformClasses IGraphics::PlatformClasses
*/

/**  The lowest level base class of an IGraphics context */
class IGraphics
#ifdef AAX_API
: public IPlugAAXView_Interface
#endif
{

public:
#pragma mark - IGraphics drawing API implementation
  //These are NanoVG only, may be refactored
  virtual void BeginFrame() {};
  virtual void EndFrame() {};
  virtual void ViewInitialized(void* pLayer) {};
  //

  /** Called by the platform IGraphics class when UI created and when moving to a new screen with different DPI, implementations in draw class must call the base implementation
   * @param scale An integer specifying the scale of the display, typically 2 on a macOS retina screen */
  virtual void SetDisplayScale(int scale) { mDisplayScale = (float) scale; OnDisplayScale(); };

  /** Draw an SVG image to the graphics context
   * @param svg The SVG image to the graphics context
   * @param bounds The rectangular region to draw the image in
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawSVG(ISVG& svg, const IRECT& bounds, const IBlend* pBlend = 0) = 0;

  /** Draw an SVG image to the graphics context with rotation 
   * @param svg The SVG image to draw to the graphics context
   * @param destCentreX The X coordinate in the graphics context of the centre point at which to rotate the image around. /todo check this
   * @param destCentreY The Y coordinate in the graphics context of the centre point at which to rotate the image around. /todo check this
   * @param width /todo
   * @param height /todo
   * @param angle The angle to rotate the bitmap mask at in degrees clockwise
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawRotatedSVG(ISVG& svg, float destCentreX, float destCentreY, float width, float height, double angle, const IBlend* pBlend = 0) = 0;

  /** Draw a bitmap (raster) image to the graphics context
   * @param bitmap The bitmap image to draw to the graphics context
   * @param bounds The rectangular region to draw the image in
   * @param srcX The X coordinate in the graphics context in the source image to draw from
   * @param srcY The Y coordinate in the graphics context in the source image to draw from
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawBitmap(IBitmap& bitmap, const IRECT& bounds, int srcX, int srcY, const IBlend* pBlend = 0) = 0;

  /** Draw a bitmap (raster) image to the graphics context with rotation
   * @param bitmap The bitmap image to draw to the graphics context
   * @param destCentreX The X coordinate in the graphics context of the centre point at which to rotate the image around. /todo check this
   * @param destCentreY The Y coordinate in the graphics context of the centre point at which to rotate the image around. /todo check this
   * @param angle The angle of rotation in degrees
   * @param yOffsetZeroDeg /todo
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawRotatedBitmap(IBitmap& bitmap, int destCentreX, int destCentreY, double angle, int yOffsetZeroDeg = 0, const IBlend* pBlend = 0) = 0;

  /** Draw a rotated, masked bitmap to the graphics context
   * @param base The base bitmap image to draw to the graphics context /todo explain base
   * @param mask The mask bitmap image to draw to the graphics context /todo explain mask
   * @param top The bitmap image to draw to the graphics context /todo explain top
   * @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw
   * @param angle The angle to rotate the bitmap mask at in degrees clockwise
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawRotatedMask(IBitmap& base, IBitmap& mask, IBitmap& top, int x, int y, double angle, const IBlend* pBlend = 0) = 0;

  /** Fill a point/pixel with a color. /todo what about high DPI displays
   * @param color The color to fill the point with
   * @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawPoint(const IColor& color, float x, float y, const IBlend* pBlend = 0) = 0;

  /** Fill a point/pixel with a color. /todo what about high DPI displays 
   * @param color The color to fill the shape with
   * @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw */
  virtual void ForcePixel(const IColor& color, int x, int y) = 0;

  /** @param color The color to draw the shape with
   * @param x1 The X coordinate in the graphics context of the start of the line
   * @param y1 The Y coordinate in the graphics context of the start of the line
   * @param x2 The X coordinate in the graphics context of the end of the line
   * @param y2 The Y coordinate in the graphics context of the end of the line
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawLine(const IColor& color, float x1, float y1, float x2, float y2, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param x1 The X coordinate in the graphics context of the first vertex
   * @param y1 The Y coordinate in the graphics context of the first vertex
   * @param x2 The X coordinate in the graphics context of the second vertex
   * @param y2 The Y coordinate in the graphics context of the second vertex
   * @param x3 The X coordinate in the graphics context of the third vertex
   * @param y3 The Y coordinate in the graphics context of the third vertex
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawTriangle(const IColor& color, float x1, float y1, float x2, float y2, float x3, float y3, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param bounds The rectangular region to draw the shape in
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawRect(const IColor& color, const IRECT& bounds, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param bounds The rectangular region to draw the shape in
   * @param cr The corner width in pixels
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawRoundRect(const IColor& color, const IRECT& bounds, float cr = 5.f, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param cx The X coordinate in the graphics context of the centre of the circle on which the arc lies
   * @param cy The Y coordinate in the graphics context of the centre of the circle on which the arc lies
   * @param r The radius of the circle on which the arc lies
   * @param aMin the start angle  of the arc at in degrees clockwise where 0 is up
   * @param aMax the end angle  of the arc at in degrees clockwise where 0 is up
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawArc(const IColor& color, float cx, float cy, float r, float aMin, float aMax, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param cx The X coordinate in the graphics context of the centre of the circle
   * @param cy The Y coordinate in the graphics context of the centre of the circle
   * @param r The radius of the circle
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawCircle(const IColor& color, float cx, float cy, float r, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param x Pointer to the first element in an array of x coordinates for the vertices of the polygon
   * @param y Pointer to the first element in an array of y coordinates for the vertices of the polygon
   * @param nPoints The number of points in the coordinate arrays
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawConvexPolygon(const IColor& color, float* x, float* y, int nPoints, const IBlend* pBlend = 0) = 0;

  /** @param color The color to draw the shape with
   * @param bounds The rectangular region to draw the shape in
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void DrawDottedRect(const IColor& color, const IRECT& bounds, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param x1 The X coordinate in the graphics context of the first vertex
   * @param y1 The Y coordinate in the graphics context of the first vertex
   * @param x2 The X coordinate in the graphics context of the second vertex
   * @param y2 The Y coordinate in the graphics context of the second vertex
   * @param x3 The X coordinate in the graphics context of the third vertex
   * @param y3 The Y coordinate in the graphics context of the third vertex
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillTriangle(const IColor& color, float x1, float y1, float x2, float y2, float x3, float y3, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param bounds The rectangular region to fill the shape in
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillRect(const IColor& color, const IRECT& bounds, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param bounds The rectangular region to fill the shape in
   * @param cr The corner radius in pixels
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillRoundRect(const IColor& color, const IRECT& bounds, float cr = 5.f, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param cx The X coordinate in the graphics context of the centre of the circle
   * @param cy The Y coordinate in the graphics context of the centre of the circle
   * @param r The radius of the circle
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillCircle(const IColor& color, float cx, float cy, float r, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param cx The X coordinate in the graphics context of the centre of the circle on which the arc lies
   * @param cy The Y coordinate in the graphics context of the centre of the circle on which the arc lies
   * @param r The radius of the circle on which the arc lies
   * @param aMin the start angle  of the arc at in degrees clockwise where 0 is up
   * @param aMax the end angle  of the arc at in degrees clockwise where 0 is up
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillArc(const IColor& color, float cx, float cy, float r, float aMin, float aMax, const IBlend* pBlend = 0) = 0;

  /** @param color The color to fill the shape with
   * @param x Pointer to the first element in an array of x coordinates for the vertices of the polygon
   * @param y Pointer to the first element in an array of y coordinates for the vertices of the polygon
   * @param nPoints The number of points in the coordinate arrays
   * @param pBlend Optional blend method, see IBlend documentation */
  virtual void FillConvexPolygon(const IColor& color, float* x, float* y, int nPoints, const IBlend* pBlend = 0) = 0;

  /** @param text An IText struct containing font and text properties and layout info
   * @param str The text string to draw in the graphics context
   * @param destRect Either should contain the rectangular region in the graphics where you would like to draw the text (when measure = false)
   * or if measure == true, after calling the method this IRECT will be updated with the rectangular region the text will occupy
   * @param measure Pass true if you wish to measure the rectangular region this text will occupy, rather than draw
   * @return true on valid input data /todo check this */
  virtual bool DrawText(const IText& text, const char* str, IRECT& destRect, bool measure = false) = 0;

  /** @param text An IText struct containing font and text properties and layout info
   * @param str The text string to draw in the graphics context
   * @param destRect after calling the method this IRECT will be updated with the rectangular region the text will occupy
   * @return true on valid input data /todo check this */
  virtual bool MeasureText(const IText& text, const char* str, IRECT& destRect) = 0;

  /** Get the color of a pixel in the graphics context 
   * @param x The X coordinate in the graphics context of the pixel
   * @param y The Y coordinate in the graphics context of the pixel
   * @return An IColor specifiying the color of the pixel at x,y */
  virtual IColor GetPoint(int x, int y)  = 0;

  /** Gets a void pointer to IGraphics Draw Class context data (e.g raw framebuffer). 
   * See draw class implementation headers (e.g. IGraphicsLice.h) for what you can cast the void pointer to */
   virtual void* GetData() = 0;

  /** @return A string representing the Drawing API in use e.g. "LICE" */
  virtual const char* GetDrawingAPIStr() = 0;

  /** This is overridden in some IGraphics drawing classes to clip drawing to a rectangular region
   * @param bounds The rectangular region to clip  */
  inline virtual void ClipRegion(const IRECT& rect) {};

  /** This is overridden in some IGraphics drawing classes to reset clipping after drawing a shape */
  inline virtual void ResetClipRegion() {};

#pragma mark - IGraphics drawing API implementation (bitmap handling)
  virtual IBitmap LoadBitmap(const char* name, int nStates = 1, bool framesAreHorizontal = false);
  virtual IBitmap ScaleBitmap(const IBitmap& srcbitmap, const char* cacheName, int targetScale);
  //virtual IBitmap CropBitmap(const IBitmap& bitmap, const IRECT& bounds, const char* name, int targetScale) = 0;
  virtual void RetainBitmap(const IBitmap& bitmap, const char* cacheName);
  virtual void ReleaseBitmap(const IBitmap& bitmap);
  IBitmap GetScaledBitmap(IBitmap& src);

  /**
   */
  virtual void OnDisplayScale();

  /** Called by some drawing API classes to finally blit the draw bitmap onto the screen */


  /**
   */
  virtual void RenderDrawBitmap() {}

#pragma mark - IGraphics base implementation - drawing helpers

  /** Draws a bitmap into the graphics context

 @param bitmap - the bitmap to draw
   * @param bounds - where to draw the bitmap
   * @param bmpState - the frame of the bitmap to draw
   * @param pBlend - blend operation
  */
  void DrawBitmap(IBitmap& bitmap, const IRECT& bounds, int bmpState = 1, const IBlend* pBlend = 0);

  /** Draws monospace bitmapped text. Useful for identical looking text on multiple platforms.
   * @param bitmap the bitmap containing glyphs to draw
   * @param bounds where to draw the bitmap
   * @param text text properties (note - many of these are irrelevant for bitmapped text)
   * @param pBlend blend operation
   * @param str the string to draw
   * @param vCenter centre the text vertically
   * @param multiline should the text spill onto multiple lines
   * @param charWidth how wide is a character in the bitmap
   * @param charHeight how high is a character in the bitmap
   * @param charOffset what is the offset between characters drawn
  */
  void DrawBitmapedText(IBitmap& bitmap, IRECT& bounds, IText& text, IBlend* pBlend, const char* str, bool vCenter = true, bool multiline = false, int charWidth = 6, int charHeight = 12, int charOffset = 0);

  /** @param color The color to fill the shape with
   * @param bounds The rectangular region to fill the shape in
   * @param x The X coordinate in the graphics context at which to draw
   * @param pBlend Optional blend method, see IBlend documentation
  */
  void DrawVerticalLine(const IColor& color, const IRECT& bounds, float x, const IBlend* pBlend = 0);

  /** @param color The color to fill the shape with
   * @param bounds The rectangular region to fill the shape in
   * @param y The Y coordinate in the graphics context at which to draw
   * @param pBlend Optional blend method, see IBlend documentation
  */
  void DrawHorizontalLine(const IColor& color, const IRECT& bounds, float y, const IBlend* pBlend = 0);

  /** @param color The color to fill the shape with
   * @param xi <#xi>
   * @param yLo <#yLo>
   * @param yHi <#yHi>
   * @param pBlend Optional blend method, see IBlend documentation
  */
  void DrawVerticalLine(const IColor& color, float xi, float yLo, float yHi, const IBlend* pBlend = 0);

  /** @param color The color to fill the shape with
   * @param yi <#yi>
   * @param xLo <#xLo>
   * @param xHi <#xHi>
   * @param pBlend Optional blend method, see IBlend documentation
  */
  void DrawHorizontalLine(const IColor& color, float yi, float xLo, float xHi, const IBlend* pBlend = 0);

  
  /**
   Helper function to draw a radial line, useful for pointers on dials
   
   * @param color the color of the line
   * @param cx centre point x coordinate
   * @param cy centre point y coordinate
   * @param angle The angle to draw at in degrees clockwise where 0 is up
   * @param rMin minima of the radial line (distance from cx,cy)
   * @param rMax maxima of the radial line (distance from cx,cy)
   * @param pBlend blend operation
   */
  void DrawRadialLine(const IColor& color, float cx, float cy, float angle, float rMin, float rMax, const IBlend* pBlend = 0);
  
  /**
  * @param color The color to fill the shape with
   * @param bounds The rectangular region to fill the shape in
   * @param gridSizeH <#gridSizeH>
   * @param gridSizeV <#gridSizeV>
   * @param pBlend Optional blend method, see IBlend documentation
   */
  void DrawGrid(const IColor& color, const IRECT& bounds, int gridSizeH, int gridSizeV, const IBlend* pBlend = 0);
  
#pragma mark - IGraphics drawing API Path support
  
  virtual bool HasPathSupport() const { return false; }
  
  virtual void PathStart() {}
  virtual void PathClose() {}
  
  void PathLine(float x1, float y1, float x2, float y2)
  {
    PathMoveTo(x1, y1);
    PathLineTo(x2, y2);
  }
  
  virtual void PathTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {}
  virtual void PathRect(const IRECT& rect) {}
  virtual void PathRoundRect(const IRECT& bounds, float cr = 5.f) {}
  virtual void PathArc(float cx, float cy, float r, float aMin, float aMax) {}
  virtual void PathCircle(float cx, float cy, float r) {}
  virtual void PathConvexPolygon(float* x, float* y, int nPoints) {}
  
  virtual void PathMoveTo(float x, float y) {}
  virtual void PathLineTo(float x, float y) {}
  virtual void PathCurveTo(float x1, float y1, float x2, float y2, float x3, float y3) {}
  
  virtual void PathStroke(const IPattern& pattern, float thickness, const IStrokeOptions& options = IStrokeOptions(), const IBlend* pBlend = 0) {}
  virtual void PathFill(const IPattern& pattern, const IFillOptions& options = IFillOptions(), const IBlend* pBlend = 0) {}

#pragma mark - IGraphics platform implementation
  /** */ 
  virtual void HideMouseCursor() {};

  /** */
  virtual void ShowMouseCursor() {};

  /** */ 
  virtual void ForceEndUserEdit() = 0;

  /** @param w <#w>
   * @param h <#h>
   * @param scale <#scale>  */
  virtual void Resize(int w, int h, float scale);

  /** @param pParentWnd <#pParentWnd>
  */
  virtual void* OpenWindow(void* pParentWnd) = 0;

  /** */ 
  virtual void CloseWindow() = 0;

  /** */ 
  virtual void* GetWindow() = 0;

  /** @param str <#str>
   * @return <#return value>
  */
  virtual bool GetTextFromClipboard(WDL_String& str) = 0;

  /** */ 
  virtual void UpdateTooltips() = 0;

  /** @param str <#str>
   * @param caption <#caption>
   * @param type <#type>
   * @return <#return value> */
  virtual int ShowMessageBox(const char* str, const char* caption, int type) = 0;

  /** @param menu <#menu>
   * @param textRect <#textRect>
   * @return <#return value> */
  virtual IPopupMenu* CreateIPopupMenu(IPopupMenu& menu, IRECT& textRect) = 0;

  /** @param pControl <#pControl>
   * @param text <#text>
   * @param textRect <#textRect>
   * @param "" <#"">
   * @param pParam <#pParam>  */
  virtual void CreateTextEntry(IControl& control, const IText& text, const IRECT& textRect, const char* str = "") = 0;

  /** @param filename <#filename>
   * @param path <#path>
   * @param action <#action>
   * @param extensions <#extensions> */
  virtual void PromptForFile(WDL_String& filename, WDL_String& path, EFileAction action = kFileOpen, const char* extensions = 0) = 0;

  /** @param color When a color is chosen the IColor referenced will be updated with the new color
   * @param str The text to display in the dialog box e.g. "Please choose a color..."
   * @return /true if prompt completed successfully */
  virtual bool PromptForColor(IColor& color, const char* str = "") = 0;

  /** @param url <#url>
   * @param msgWindowTitle <#msgWindowTitle>
   * @param confirmMsg <#confirmMsg>
   * @param errMsgOnFailure <#errMsgOnFailure>
   * @return <#return value> */
  virtual bool OpenURL(const char* url, const char* msgWindowTitle = 0, const char* confirmMsg = 0, const char* errMsgOnFailure = 0) = 0;

  /** @return A CString representing the Platform API in use e.g. "macOS" */
  virtual const char* GetPlatformAPIStr() { return ""; }

  virtual bool WindowIsOpen() { return GetWindow(); }

  /** @param path <#path> */
  virtual void HostPath(WDL_String& path) = 0;

  /** @param path <#path> */
  virtual void PluginPath(WDL_String& path) = 0;

  /** @param path <#path> */
  virtual void DesktopPath(WDL_String& path) = 0;

  /** @param path <#path> */
  virtual void UserHomePath(WDL_String& path) = 0;

  /** @param path <#path>
   * @param isSystem <#isSystem> */
  virtual void AppSupportPath(WDL_String& path, bool isSystem = false) = 0;

  /** @param path <#path> */
  virtual void SandboxSafeAppSupportPath(WDL_String& path) = 0;

  /** @param path <#path>
   * @param isSystem <#isSystem> */
  virtual void VST3PresetsPath(WDL_String& path, const char* mfrName, const char* pluginName, bool isSystem = true) { path.Set(""); }

  /** @param path <#path>
   * @param select <#select>
   * @return <#return value> */
  virtual bool RevealPathInExplorerOrFinder(WDL_String& path, bool select = false) = 0;

  /* Used on Windows to set the HINSTANCE handle, which allows graphics APIs to load resources from the binary
   * @param instance <#instance> */
  virtual void SetPlatformInstance(void* pInstance) {}


  /** */
  virtual void* GetPlatformInstance() { return nullptr; }

  /**
   Used with IGraphicsLice (possibly others) in order to set the core graphics draw context on macOS and the GDI HDC draw context handle on Windows
   On macOS, this is called by the platform IGraphics class IGraphicsMac, on Windows it is called by the drawing class e.g. IGraphicsLice.

   * @param pContext <#pContext> */
  virtual void SetPlatformContext(void* pContext) { mPlatformContext = pContext; }

  /** @return <#return value> */
  void* GetPlatformContext() { return mPlatformContext; }

  /** Try to ascertain the full, absolute path of a resource based on it's name

   * @param name <#name>
   * @param type <#type>
   * @param result <#result>
   * @return \c True on success */
  virtual bool OSFindResource(const char* name, const char* type, WDL_String& result) = 0;

#pragma mark - IGraphics base implementation
  IGraphics(IDelegate& dlg, int w, int h, int fps = 0);
  virtual ~IGraphics();

  /** @param bounds The rectangular region which will be added to to mark what is dirty in the context
   * @return <#return value> */
  bool IsDirty(IRECT& rect);

  /** @param bounds The rectangular region to draw */
  virtual void Draw(const IRECT& rect);

  /** @param name <#name>
   * @return <#return value> */
  virtual ISVG LoadSVG(const char* name); // TODO: correct place?

  /** This method is called after interacting with a control, so that any other controls linked to the same parameter index, will also be set dirty, and have their values updated.
   * @param caller The control that triggered the parameter change. */
  void UpdatePeers(IControl* pCaller);

  /** @param pControl <#pControl>
   * @param pParam <#pParam>
   * @param textRect <#textRect> */
  void PromptUserInput(IControl& control, IRECT& textRect);

  /** @param pControl <#pControl>
   * @param pParam <#pParam>
   * @param txt <#txt> */
  void SetControlValueFromStringAfterPrompt(IControl& control, const char* txt);

  /** @param menu <#menu>
   * @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw
   * @return <#return value> */
  IPopupMenu* CreateIPopupMenu(IPopupMenu& menu, float x, float y) { IRECT tempRect = IRECT(x,y,x,y); return CreateIPopupMenu(menu, tempRect); }

  /** @param strict <#strict> */
  void SetStrictDrawing(bool strict);

  /** @return <#return value> */
  int Width() const { return mWidth; }

  /** @return <#return value> */
  int Height() const { return mHeight; }

  /** @return <#return value> */
  int WindowWidth() const { return int((float) mWidth * mScale); }

  /** @return <#return value> */
  int WindowHeight() const { return int((float) mHeight * mScale); }

  /** @return <#return value> */
  int FPS() const { return mFPS; }

  /** @return <#return value> */
  float GetScale() const { return mScale; }

  /** @return <#return value> */
  float GetDisplayScale() const { return mDisplayScale; }

  /** @return <#return value> */
  IDelegate& GetDelegate() { return mDelegate; }

  void AttachBackground(const char* name);
  void AttachPanelBackground(const IColor& color);
  void AttachKeyCatcher(IControl& control);

  /** @param control <#control>
   * @return <#return value> */
  int AttachControl(IControl* pControl);

  /** @param idx The index of the control to get
   * @return A pointer to the IControl object at idx */
  IControl* GetControl(int idx) { return mControls.Get(idx); }

  /** @return The number of controls that have been added to this graphics context */
  int NControls() const { return mControls.GetSize(); }

  /** @param paramIdx <#paramIdx>
   * @param hide <#hide> */
  void HideControl(int paramIdx, bool hide);

  /** @param paramIdx <#paramIdx>
   * @param gray <#gray>
  */
  void GrayOutControl(int paramIdx, bool gray);

  /** @param paramIdx <#paramIdx>
   * @param lo <#lo>
   * @param hi <#hi>
   * @param normalized <#normalized>*/
  void ClampControl(int paramIdx, double lo, double hi, bool normalized);

  /***/ 
  void SetAllControlsDirty();

  /** @param x The X coordinate in the graphics context at which the mouse event occurred
   * @param y The Y coordinate in the graphics context at which the mouse event occurred
   * @param mod IMouseMod struct contain information about the modifiers held */
  void OnMouseDown(float x, float y, const IMouseMod& mod);

  /** @param x The X coordinate in the graphics context at which the mouse event occurred
   * @param y The Y coordinate in the graphics context at which the mouse event occurred
   * @param mod IMouseMod struct contain information about the modifiers held */
  void OnMouseUp(float x, float y, const IMouseMod& mod);

  /** @param x The X coordinate in the graphics context at which the mouse event occurred
   * @param y The Y coordinate in the graphics context at which the mouse event occurred
   * @param dX Delta X value /todo explain
   * @param dY Delta Y value /todo explain
   * @param mod IMouseMod struct contain information about the modifiers held */
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod);

  /** @param x The X coordinate in the graphics context at which the mouse event occurred
   * @param y The Y coordinate in the graphics context at which the mouse event occurred
   * @param mod IMouseMod struct contain information about the modifiers held
   * @return <#return value> */
  bool OnMouseDblClick(float x, float y, const IMouseMod& mod);

  /** @param x The X coordinate in the graphics context at which the mouse event occurred
   * @param y The Y coordinate in the graphics context at which the mouse event occurred
   * @param mod IMouseMod struct contain information about the modifiers held
   * @param delta Delta value /todo explain */
  void OnMouseWheel(float x, float y, const IMouseMod& mod, float delta);

  /** @param x The X coordinate in the graphics context of the mouse cursor at the time of the key press
   * @param y The Y coordinate in the graphics context of the mouse cursor at the time of the key press
   * @param key An integer represent the key pressed, see EIPlugKeyCodes
   * @return \c True if handled /todo check this */
  bool OnKeyDown(float x, float y, int key);

  /** @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw
   * @param mod IMouseMod struct contain information about the modifiers held
   * @return \c True if handled /todo check this */
  bool OnMouseOver(float x, float y, const IMouseMod& mod);

  /** */
  void OnMouseOut();

  /** @param str A CString with the absolute path of the dropped item
   * @param x The X coordinate in the graphics context where the drag and drop occurred
   * @param y The Y coordinate in the graphics context where the drag and drop occurred */
  void OnDrop(const char* str, float x, float y);

  /** */ 
  void OnGUIIdle();

  /** [AAX only] This can be called by the ProTools API class (e.g. IPlugAAX) in order to ascertain the parameter linked to the control under the mouse.
   * The purpose is to facillitate ProTool's special contextual menus (for configuring parameter automation)
   * @param x The X coordinate in the graphics context to check
   * @param y The Y coordinate in the graphics contextto check
   * @return An integer representing the parameter index that was found (or -1 if not found) */
  int GetParamIdxForPTAutomation(float x, float y);

  /** [AAX only]
   * @return An integer representing the last clicked parameter index (or -1 if none) */
  int GetLastClickedParamForPTAutomation();

  /** [AAX only] Called by the 
   * @param paramIdx The index of the parameter to highlight
   * @param isHighlighted /c true if the parameter should be highlighted 
   * @param color An integer corresponding to AAX_EParameterHighlight /todo check Enum name */
  void SetPTParameterHighlight(int paramIdx, bool isHighlighted, int color);

  /** [VST3 primarily]
   * @param controlIdx <#controlIdx>
   * @param paramIdx <#paramIdx>
   * @param x The X coordinate in the graphics context at which to draw
   * @param y The Y coordinate in the graphics context at which to draw */
  void PopupHostContextMenuForParam(int controlIdx, int paramIdx, float x, float y);

  /** @param enable Set \c True if you want to handle mouse over messages. Note: this may increase the amount CPU usage if you redraw on mouse overs etc */
  void HandleMouseOver(bool canHandle) { mHandleMouseOver = canHandle; }

  /***/ 
  void ReleaseMouseCapture();

  /** @param enable Set \c True to enable tool tips when the user mouses over a control */
  void EnableTooltips(bool enable);

  /** Call this method in order to create tool tips for every IControl that show the associated parameter's name */ 
  void AssignParamNameToolTips();

  /** @param enable Set \c True if you wish to draw the rectangular region of the graphics context occupied by each IControl in mControls  */
  inline void ShowControlBounds(bool enable) { mShowControlBounds = enable; }

  /** @param enable Set \c True if you wish to show the rectangular region that is drawn on each frame, in order to debug redraw problems */
  inline void ShowAreaDrawn(bool enable) { mShowAreaDrawn = enable; }

  /** Live edit mode allows you to relocate controls at runtime in debug builds and save the locations to a predefined file (e.g. main plugin .cpp file) /todo we need a separate page for liveedit info
   * @param enable Set \c True if you wish to enable live editing mode
   * @param file The absolute path of the file which contains the layout info (correctly tagged) for live editing
   * @param gridsize The size of the layout grid in pixels */
  void EnableLiveEdit(bool enable, const char* file = 0, int gridsize = 10);

  /** Return the rectangular region of the graphics context marked for drawing
   * @return An IRECT that corresponds to the rectangular region currently marked for drawing */
  IRECT GetDrawRect() const { return mDrawRECT; }

  /** Returns an IRECT that represents the entire UI bounds
   * This is useful for programatically arranging UI elements by slicing up the IRECT using the various IRECT methods
   * @return An IRECT that corresponds to the entire UI area, with, L = 0, T = 0, R = Width() and B  = Height() */
  IRECT GetBounds() const { return IRECT(0.f, 0.f, (float) Width(), (float) Height()); }

  /** @return \c True if the context can handle mouse overs */
  bool CanHandleMouseOver() const { return mHandleMouseOver; }

  /** @return An integer representing the control index in IGraphics::mControls which the mouse is over, or -1 if it is not */
  inline int GetMouseOver() const { return mMouseOver; }

  /** @return \c True if tool tips are enabled */
  inline bool TooltipsEnabled() const { return mEnableTooltips; }

  /** @param name The name of the font to load */
  virtual void LoadFont(const char* name);

protected:
  IDelegate& mDelegate;

  virtual APIBitmap* LoadAPIBitmap(const WDL_String& resourcePath, int scale) = 0;
  //virtual void* CreateAPIBitmap(int w, int h) = 0;
  virtual APIBitmap* ScaleAPIBitmap(const APIBitmap* pBitmap, int scale) = 0;

  inline void SearchNextScale(int& sourceScale, int targetScale);
  bool SearchImageResource(const char* name, const char* type, WDL_String& result, int targetScale, int& sourceScale);
  APIBitmap* SearchBitmapInCache(const char* name, int targetScale, int& sourceScale);

  WDL_PtrList<IControl> mControls;
  IRECT mDrawRECT;
  void* mPlatformContext = nullptr;
  bool mCursorHidden = false;
private:
  int GetMouseControlIdx(float x, float y, bool mo = false);

  int mWidth;
  int mHeight;
  int mFPS;
  float mDisplayScale = 1.f; // the scaling of the display that the ui is currently on e.g. 2 for retina
  float mScale = 1.f; // scale deviation from dlg-in width and height i.e .stretching the gui by dragging
  int mIdleTicks = 0;
  int mMouseCapture = -1;
  int mMouseOver = -1;
  int mLastClickedParam = kNoParameter;
  bool mHandleMouseOver = false;
  bool mStrict = true;
  bool mEnableTooltips = false;
  bool mShowControlBounds = false;
  bool mShowAreaDrawn = false;
  IControl* mKeyCatcher = nullptr;

#if !defined(NDEBUG) && defined(APP_API)
  IControl* mLiveEdit = nullptr;
#endif

#ifdef IGRAPHICS_FREETYPE
protected:
  FT_Library mFTLibrary = nullptr;
  FT_Face mFTFace = nullptr;
#endif

  friend class IGraphicsLiveEdit;
};

