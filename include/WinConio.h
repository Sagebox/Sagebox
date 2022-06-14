#pragma once

namespace Sage
{
    // WinConio Structure -- Functions for Console Window Output 
    // ---------------------------------------------------------
    //
    // WinConio (referred to as "conio" in code) functions can be used to put out formatted text with colors and control the Console Mode window. 
    //
    // "{}"-style modifiers may be used to control the output, such as setting text colors, background colors, and underlined text. 
    //
    // For example,
    //
    //  conio.printf("This {r}word{/} was written in red")          -- writes the word "word" in red text .
    //  conio.printf("This {bg=y}{r}word{/}{/} was written in red") -- writes the word "word" in red text with a yellow background.
    //  conio.printf("This {rev}word{/}{/} was written in reverse") -- writes the word "word" in reversed text 
    //  conio.printf("This {u}word{/}{/} was written underlined")   -- writes the word "word" in underlined text.
    //
    // --> Note: WinConio "{}"-style modifiers are not 100% compatible with other Sagebox/Quick C++ modifiers.
    //           For example colors like "yellow" are typically compatible, but "{rev}" (to reverse text)" is only used for WinConio.
    //
    // --> Note: WinConio is useful for the Console Window only.  It does not print out to stdio.  
    //     Therefore, if stdio is re-routed conio will still print the the Console Window.
    //     if a Console Window does not exist (such as a pure Windows program or Quick C++ project with no console mode window), WinnConio functions
    //     will have no effect.
    //
    // --> Note: Tabs are set with tabstops every 4 spaces, and can be used for formatting.  See also "x=" below to set specific X character 
    //           settings for aligned text.
    //
    // A complete list of "{}"-style modifiers for WinConIo
    // ----------------------------------------------------
    //
    //      black       - color black (text or background)    
    //      darkblue    - color dark blue (text or background)       
    //      db          - color dark blue (text or background)       
    //      darkgreen   - color dark green (text or background)       
    //      dg          - color dark green (text or background)       
    //      darkcyan    - color dark cyan (text or background)       
    //      dc          - color dark cyan (text or background)       
    //      darkRed     - color dark red(text or background)      
    //      dr          - color dark red (text or background)       
    //      darkpurple  - color dark purple (text or background)      
    //      dp          - color (text or background)       
    //      darkmagenta - color dark purple (text or background)      
    //      dm          - color dark magenta (text or background)       
    //      darkYellow  - color dark yellow (text or background)       
    //      dy          - color dark yellow (text or background)       
    //      gray        - color gray (text or background)       
    //      grey        - color gray (text or background)       
    //      blue        - color blue (text or background)       
    //      b           - color blue (text or background)       
    //      green       - color green (text or background)       
    //      g           - color green (text or background)       
    //      cyan        - color cyan (text or background)       
    //      c           - color cyan (text or background)       
    //      red         - color red (text or background)       
    //      r           - color red (text or background)       
    //      purple      - color purple (text or background)       
    //      p           - color purple (text or background)       
    //      magenta     - color magenta (text or background)       
    //      m           - color m (text or background)       
    //      Yellow      - color yellow (text or background)       
    //      y           - color yellow (text or background)       
    //      w           - color white (text or background)       
    //      white       - color white (text or background)       
    //      u           - underline text
    //      vl          - left bar on character (i.e. vertical left)
    //      vr          - right bar on character (i.e. vertical top)
    //      ht          - topline text (i.e. underline, but on top) (i.e. horizontal top)
    //      hb          - same as underline (i.e. horizontal bottom)
    //      rev         - reverse text colors (foreground color = background color; background color = foreground color)
    //      bg=         - Set background color next text (i.e. {bg=blue})
    //      lbg=        - Set background color for entire line (i.e. {lbg=blue})
    //      X=          - Set X character position in line (for aligning text), i.e. "This is at {x=40} column 40"
    //
    struct WinConio
    {
        WinConio & operator << (const char * x)        ;
        WinConio & operator << (char x)                ;
        WinConio & operator << (std::string & cs)      ;
        WinConio & operator << (CString & cs)          ;
        WinConio & operator << (int x)                 ;
        WinConio & operator << (unsigned int x)        ;
        WinConio & operator << (float x)               ;
        WinConio & operator << (double x)              ;
        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(const char * sText)                          ;

        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(int iX,int iY,const char * sText)            ;

        /// <summary>
        /// Write text to the console window.  "{}"-style modifiers may be used, such as conio.Write("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// Write(MyPoint,"This message was written at character location 10,30"); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <returns></returns>
        bool Write(POINT pLoc,const char * sText)               ;

        /// <summary>
        /// Sets the font size of the Console Window.  This change the font for all characters in the Consolle Window simultaneously.
        /// <para></para>
        /// Typically, 14-16 is a good number, where numbers less than 14 start to represent smaller fonts, and numbers larger than 16 represent big fonts.
        /// <para></para>&#160;&#160;&#160;
        /// When setting the font, this will not keep the available characters in the console window the same, keeping the Console Window the same physical size.  Therefore, use SetFont() before setting the window size. 
        /// <para></para>&#160;&#160;&#160;
        /// You can also use the MouseWheel when pressing the control-key -- this will keep the number of characters in the window the same while changing the size of the Console Window.  This method will keep text and other
        /// aligned text still centered and aligned, where using SetFont() will affect any centering and text alignment.
        /// </summary>
        /// <param name="iSize">= Size of the new font</param>
        /// <returns></returns>
        bool SetFontSize(int iSize)                             ;

        /// <summary>
        /// Sets the Text (i.e. foreground) color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetFgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the text color with conio.printf() or Write(), such as conio.printf("This {red}word{/} is written in red.")
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the text color</param>
        /// <param name="sColor">The color name to set for the text color</param>
        /// <returns></returns>
        bool SetFgColor(int iColor)                             ;

        /// <summary>
        /// Sets the Text (i.e. foreground) color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetFgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the text color with conio.printf() or Write(), such as conio.printf("This {red}word{/} is written in red.")
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the text color</param>
        /// <param name="sColor">The color name to set for the text color</param>
        /// <returns></returns>
        bool SetFgColor(const char * sColor)                    ;

        /// <summary>
        /// Sets the Background color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetBgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the background color with conio.printf() or Write(), such as conio.printf("This {bg=blue}word{/} has a blue background.")
        /// <para></para>--> Note: You can also use "{rev}some text{/}" as a quick way to reverse the colors for selected text.
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the background color</param>
        /// <param name="sColor">The color name to set for the background color</param>
        /// <returns></returns>
        bool SetBgColor(int iColor,bool bFillBg = true)         ;

        /// <summary>
        /// Sets the Background color of text in the Console Window. This affects the next printed characters.
        /// <para></para>&#160;&#160;&#160;
        /// Use a text color, such as "White", "blue", "db" (for darkblue"), "darkred", etc.  to set the text color. 
        /// <para></para>You can also use a number such as 0x04 (for red), etc. The value of the numbers can be obtained by searching the internet for "Win32 Console Mode Text Colors". 
        /// <para></para>GetBgColor() can be used to retrieve the current color, which returns it as a numerical value specific to the Console Window.
        /// <para></para>--> Note: You can embed the background color with conio.printf() or Write(), such as conio.printf("This {bg=blue}word{/} has a blue background.")
        /// <para></para>--> Note: You can also use "{rev}some text{/}" as a quick way to reverse the colors for selected text.
        /// </summary>
        /// <param name="iColor">= The color code (0x00-0x0F) to set for the background color</param>
        /// <param name="sColor">The color name to set for the background color</param>
        /// <returns></returns>
        bool SetBgColor(const char * sColor,bool bFillBg = true);

        /// <summary>
        /// Sets the title of the Console Window in the Title Bar. Use "" or nullptr for no title.
        /// </summary>
        /// <param name="sTitle"></param>
        /// <returns></returns>
        bool SetWindowTitle(const char * sTitle);

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(bool bResetCursor = true)                      ;

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(int iColor,bool bResetCursor = true)           ;

        /// <summary>
        /// Clears the console window with the current (or specified background color), then sets the cursor to {0,0} (unless bResetCursor = false)
        /// <para></para>&#160;&#160;&#160;
        /// If a color is specified (with a color value or color name), the background text color is also set to this color.
        /// <para></para>
        /// Examples
        /// <para></para>&#160;&#160;&#160; Cls("blue"); -- Clears the screen to blue, sets cursor at {0,0}
        /// <para></para>&#160;&#160;&#160; Cls(0x10);  -- Clears the screen to blue, does not reset cursor
        /// <para></para>&#160;&#160;&#160; Cls();      -- Clears the screen with current BG color, sets cursor to {0,0}
        /// </summary>
        /// <param name="iColor"> = Sets the CLS background color (optional)</param>
        /// <param name="sColor"> = Sets the CLS background color by name (optional)</param>
        /// <param name="bResetCursor">= Sets the cursor to {0,0}} (default) (optional)</param>
        /// <returns></returns>
        bool Cls(const char * sColor,bool bResetCursor = true)  ;

        /// <summary>
        /// Sets the background color of the entire line.  This will set the background color without erasing text to the color
        /// specified, which may be a numeric background value, or a string for the color (i.e. "Blue","darkblue","red", etc.)
        /// </summary>
        /// <param name="sColor">Name of the color for the background</param>
        /// <param name="iColor">Numeric code for the background color</param>
        /// <returns></returns>
        bool SetBgLine(const char * sColor)                     ;

        /// <summary>
        /// Sets the background color of the entire line.  This will set the background color without erasing text to the color
        /// specified, which may be a numeric background value, or a string for the color (i.e. "Blue","darkblue","red", etc.)
        /// </summary>
        /// <param name="sColor">Name of the color for the background</param>
        /// <param name="iColor">Numeric code for the background color</param>
        /// <returns></returns>
        bool SetBgLine(int iColor)                              ;


        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(const char * Format,...)                    ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(int iX,int iY,const char * Format,...)      ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool printf(POINT pLoc,const char * Format,...)         ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(const char * Format,...)                    ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(int iX,int iY,const char * Format,...)      ;

        /// <summary>
        /// Print to the console window, just as printf(), but with some options.  "{}"-style modifiers may be used, such as conio.printf("This {r}{u}word{/}{/} is red and underlined"); 
        /// <para></para>&#160;&#160;&#160;You can also specify X,Y coordinates beforehand, such as:
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(10,30,"This message was written at character location 10,30"); 
        /// <para></para>&#160;&#160;&#160;&#160;&#160;&#160;
        /// conio.printf(MyPoint,"This message was written at character location %d,%d",iX,iY); 
        /// <para></para>
        /// --> Note: the maximum length of input text is roughly 2k, but may be increased in a future update
        /// </summary>
        /// <param name="sText"> = text to write to the window</param>
        /// <param name="..."> = standard printf parameters for formatted text (i.e. variadic arguments)</param>
        /// <returns></returns>
        bool __printf(POINT pLoc,const char * Format,...)         ;

        /// <summary>
        /// Prints text in a box in the console window.  Pad with spaces to create distance between left and right borders.
        /// <para></para>
        /// Use text-based foreground and background colors to make the box more distinctive, such as "blue" for background and "white" for foreground. 
        /// <para></para>
        /// Any combination of defined conio colors can be used (i.e. "r","red","g","m", etc.)
        /// </summary>
        /// <param name="sText">- Text to print to console.  i.e. "MyText" or "    My Text     ", etc. </param>
        /// <param name="sFgColor">- [optional] foreground color to use.  Defaults to bright white (i.e. "w')</param>
        /// <param name="sBgColor">- [optional] background color.  Defaults to current background color of console window.</param>
        /// <returns></returns>
        bool Box(const char * sText,const char * sFgColor = nullptr,const char * sBgColor = nullptr);

        /// <summary>
        /// Returns background color.  This is the text attribute ANDed with 0xF0 to form only the background color
        /// </summary>
        /// <returns>Console-Based background color</returns>
        WORD GetBgColor()                                       ;
        
        /// <summary>
        /// Returns foreground color.  This is the text attribute ANDed with 0x0F to form only the foreground color
        /// </summary>
        /// <returns>Console-Based foreground color</returns>
        WORD GetFgColor()                                       ;

        /// <summary>
        /// Returns the console-mode text attribute.  This includes the background color, text color, and other attributes (such as underline, etc.)
        /// <para></para>&#160;&#160;&#160;AND the attribute with 0xF0 to obtain the background color; AND with 0x0F to obtaint the foreground color.
        /// </summary>
        /// <returns>Console attribute color</returns>
        WORD GetTextAttribute()                                 ;

        /// <summary>
        /// Set the Cursor to the (X,Y) character position in the Console Window.
        /// <para></para>&#160;&#160;&#160;    
        /// the X and Y position are character positions rather than pixel positions.
        /// <para></para>
        /// Example: GotoXY(10,20); -- Sets the cursor at character position 10,20 in the console window.
        /// </summary>
        /// <param name="iX">X position to set Cursor</param>
        /// <param name="iY">Y position to set Cursor</param>
        /// <returns></returns>
        bool GotoXY(int iX, int iY)                             ;

        /// <summary>
        /// Returns a CString with the names of colors that can be used in the "{}" Write() or printf() functions.
        /// <para></para>&#160;&#160;&#160;    
        /// For example, "printf("This is {red}red{/} or {r}red{/}") puts both "red" words in the color red on the console window.
        /// </summary>
        /// <returns></returns>
        CString GetColorNames()                                 ;

        /// <summary>
        /// Causes the console window to become visible or hidden, dependig on bShow paramater
        /// </summary>
        /// <param name="bShow"> -> true = Show console window.  false = Hide console window</param>
        /// <returns></returns>
        bool Show(bool bShow = true);

        /// <summary>
        /// Causes the console window to become hidden or visible, depending on the bHide paramater
        /// </summary>
        /// <param name="bHide"> -> true = Hide console window.  true = Show console window</param>
        /// <returns></returns>
        bool Hide(bool bHide = true);

        /// <summary>
        /// Returns TRUE if the Console Window is Visible, FALSE if the console window is hidden
        /// </summary>
        /// <returns></returns>
        bool isWindowVisible();

        /// <summary>
        /// Sets the size of the console window in pixels.
        /// <para></para>&#160;&#160;&#160;
        /// The window is sized to the nearest multiple of the character width and height, and is the internal size of the window, not the entire window (i.e. title bar, frame, etc.).
        /// <para></para>
        /// Use (iX,iY) or a SIZE value to set the window size.
        /// </summary>
        /// <param name="iWidth">Width of Window (in pixels)</param>
        /// <param name="iHeight">Height of Window (in pixels)</param>
        /// <param name="SetMaxX">When TRUE, sets the window width to remove the bottom scrollbar.  When FALSE, the max character width is not changed.</param>
        /// <returns></returns>
        bool SetWindowSize(int iWidth,int iHeight,bool SetMaxX = true); 

        /// <summary>
        /// Sets the size of the console window in pixels.
        /// <para></para>&#160;&#160;&#160;
        /// The window is sized to the nearest multiple of the character width and height, and is the internal size of the window, not the entire window (i.e. title bar, frame, etc.).
        /// <para></para>
        /// Use (iX,iY) or a SIZE value to set the window size.
        /// </summary>
        /// <param name="szSize">SIZE structure with the new window size.</param>
        /// <param name="SetMaxX">When TRUE, sets the window width to remove the bottom scrollbar.  When FALSE, the max character width is not changed.</param>
        /// <returns></returns>
        bool SetWindowSize(SIZE szSize,bool SetMaxX = true); 

        /// <summary>
        /// Set the location of the Console Widow on the desktop.
        /// <para></para>&#160;&#160;&#160;
        /// Use (iX,iY) or a POINT value to set the size (i.e. SetWIndowSize(myPoint)); 
        ///
        /// </summary>
        /// <param name="iX">X location of Console Window</param>
        /// <param name="iY">Y location of Console Window</param>
        /// <returns></returns>
        bool SetWinLocation(int iX,int iY); 

        /// <summary>
        /// Set the location of the Console Widow on the desktop.
        /// <para></para>&#160;&#160;&#160;
        /// Use (iX,iY) or a POINT value to set the size (i.e. SetWIndowSize(myPoint)); 
        ///
        /// </summary>
        /// <param name="pLoc">POINT structure with the new window location</param>
        /// <returns></returns>
        bool SetWinLocation(POINT pLoc); 

        /// <summary>
        /// Brings the Window to the top of the screen, above all other windows.  This can be useful when using a Sagebox, Quick C++ or other window with your program, to ensure
        /// the Console Window is showing above the other windows.  Use SetTopWindow() to bring the Console Window to the top permanently (i.e. can't be overlapped again)
        /// </summary>
        /// <returns></returns>
        bool SetTopWindow(); 

        /// <summary>
        /// Sets the Console Window as "topmost", which means no other window can overlap it.  Other windows (including from other applications) will be underneath the console
        /// window until the program ends or SetTopmost(false) is called to remove the TopMost status.
        /// </summary>
        /// <param name="bTopMost"> = when TRUE, sets the Console Window as "topmost".  When FALSE, resets the status so other window may overlap the console window</param>
        /// <returns></returns>
        bool SetTopmost(bool bTopMost = true); 

        /// <summary>
        /// Gets the character width of the Console Window.  This can be used for centering or otherwise aligning text.
        /// <para></para>&#160;&#160;&#160;
        /// For example, using code like conio.printf((GetMaxCharWidth()-TextWidth),-1,"This is the Text");
        /// <para></para>
        /// Will center the text on the current Y position in the console window.
        /// <para></para>&#160;&#160;&#160;--> Note: the text width is the width without and "{}" fields.  If the text has any encoded fields, the length of the text
        /// must be measured separately.
        /// </summary>
        /// <returns>Character width of Console Window</returns>
        int GetMaxCharWidth();

        /// <summary>
        /// Shows the Console Window's cursor if it is hidden.  This can also HIDE the cursor with ShowCursor(false);
        /// </summary>
        /// <param name="bShow">when TRUE, shows the cursor.  When FALSE, hides the cursor.</param>
        /// <returns></returns>
        bool ShowCursor(bool bShow = true);

        /// <summary>
        /// Hides the Console Window's cursor.  This can also SHOW the cursor with HideCursor(false);
        /// </summary>
        /// <param name="bHide">when TRUE, hides the cursor.  When FALSE, shoows the cursor.</param>
        /// <returns></returns>
        bool HideCursor(bool bHide = true);

        /// <summary>
        /// Gets an integer from the console and returns an int value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Integer entered or 0 if error or non-number value entered.</returns>
        int GetInteger(const char * sMsg, const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets an integer from the console and returns an int value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Integer entered or 0 if error or non-number value entered.</returns>
        int GetInteger(const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets a long integer from the console and returns a long value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Long value entered or 0 if error or non-number value entered.</returns>
        long GetLong(const char * sMsg, const cwfOpt & cwOpt = cwfOpt());

        /// <summary>
        /// Gets a long integer from the console and returns a long value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Long value entered or 0 if error or non-number value entered.</returns>
        long GetLong(const cwfOpt & cwOpt = cwfOpt());

        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetNumber()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetLongLong(const char * sMsg, const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetNumber()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetLongLong(const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetLongLong()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetNumber(const char * sMsg, const cwfOpt & cwOpt = cwfOpt());

        /// <summary>
        /// Gets a 64-bit 'long long' type from the console and returns a 'long long' value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// <para>Same as GetLongLong()</para>
        /// </summary>
        /// <returns>64-bit Long Long value entered or 0 if error or non-number value entered.</returns>
        long long GetNumber(const cwfOpt & cwOpt = cwfOpt());

        /// <summary>
        /// Gets a double value from the console and returns the double value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Double value entered or 0 if error or non-number value entered.</returns>
        double GetFloat(const char * sMsg, const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets a double value from the console and returns the double value. 
        /// Returns 0 if a number is not entered.
        /// <para></para>
        ///     The buffer is cleared before and after entry for error, providing for sage number entry that can be validated in a loop.
        /// </summary>
        /// <returns>Double value entered or 0 if error or non-number value entered.</returns>
        double GetFloat(const cwfOpt & cwOpt = cwfOpt()); 

        /// <summary>
        /// Gets a text line from the user and returns it as a CString object.
        /// <para></para>This provides a safe way to get a string of any length from the user.  
        /// </summary>
        /// <param name="iMax">Maximum string length (default is 2000)</param>
        /// <returns>CString of user entry, or an emptry CString if there was an error or no entry from the user.</returns>
        CString getline(int iMax = 2000); 

        /// <summary>
        /// Clears the cin input buffer. 
        /// </summary>
        /// <param name="bOnErrorOnly">when FALSE (default), the cin buffer is cleared. When TRUE, ths cin buffer is cleared only if it is an error state</param>
        void ClearCin(bool bOnErrorOnly = false);


    };

    }; // namespace Sage