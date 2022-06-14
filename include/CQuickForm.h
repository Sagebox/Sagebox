#pragma once
//#include "Sagebox.h"

namespace Sage
{
class CWindow;
class CDevControls;
class QuickFormEngine;
class CSageBox;
class CQuickForm
{
   friend CSageBox;
    QuickFormEngine * m_qForm = nullptr; 

private:
    CQuickForm(const CQuickForm & cForm) noexcept ;
    CQuickForm() noexcept;

public:
    CWindow & cMain;
    CWindow & cWin;
    CDevControls & cDev;

    /// <summary>
    /// Returns an Empty CQuickForm object.
    /// <para></para>
    /// - This is used to return passive empty Quick Form objects when required for return values where there is no active
    /// Quick form or the existing Quick Form is invalid.  For example, MyWindow.GetQuickForm() will return an empty QuickForm
    /// if MyWindow is not a member of a Quick Form (i.e. either the main window or a child window).
    /// <para></para>
    /// --> Use isValid() on a QuickForm to determine if it is valid.
    /// </summary>
    /// <returns></returns>
    static CQuickForm GetEmptyObject();

    /// <summary>
    /// Returns true if the Quickform is a valid quick form.  Otherwise false.
    /// <para></para>
    /// isValid() and isEmpty() are the same function
    /// </summary>
    /// <returns></returns>
    bool isEmpty();

    /// <summary>
    /// Returns true if the Quickform is a valid quick form.  Otherwise false.
    /// <para></para>
    /// isValid() and isEmpty() are the same function
    /// </summary>
    /// <returns></returns>
    bool isValid();

    CQuickForm(CQuickForm && cForm) noexcept;

    void Delete(); 

    // CQuickForm & operator = (CQuickForm && p2) noexcept
    // {
    //     *this = p2;
    //     return *this;
    // }

    CQuickForm(QuickFormEngine * qForm);
  
    /// <summary>
    /// Returns a tuple of references to the Main Top Window, User Window (cWin) and Dev Window (cDev)
    /// <para></para>
    /// - This can be used to easily assign values to the created windows rather than accessing the Quick Form object.
    /// <para></para>
    /// - See GetTuple2(), GetTuplePlain(), GetWinPtrs(), GetWinPtrs2(), and GetWinPtr() variations that return different tuples.
    /// <para></para>
    /// --> Example: auto [cMain, cWin, cDev] = QuickForm().GetTuple()
    /// </summary>
    /// <returns>Tuple of [cMain, cWin, cDev]</returns>
    std::tuple<CWindow &, CWindow &, CDevControls &> GetAll();

    /// <summary>
    /// Returns a tuple of references to the User Window (cWin) and Dev Window (cDev)
    /// <para></para>
    /// - This can be used to return only the cWin and cDev windows without cMain.
    /// <para></para>
    /// - In a Quick Form, cWin closing will close the entire Quick Form, and closing the Quick Form main Window
    /// will send a close message to the cWin window.
    /// <para></para>
    /// - Use GetWinPtrs2() to return a tuple with pointers instead of references.
    /// - See other Get() functions for other varations, such as GetTuple() and GetTuplePlain(), etc.
    /// <para></para>
    /// --> Example: auto [cWin, cDev] = QuickForm().GetTuple2()
    /// </summary>
    /// <returns>Tuple of [cWin, cDev]</returns>
    std::tuple<CWindow &, CDevControls &> GetWins();
    std::tuple<CWindow &, CWindow &> GetPlain();
    CWindow & GetWin();

    std::tuple<CWindow *, CWindow *, CDevControls *> GetAllWinPtrs();
    std::tuple<CWindow *, CDevControls *> GetWinPtrs();

};

} // namespace Sage
