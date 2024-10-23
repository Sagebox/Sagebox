using Sagebox.Main;
using System.Diagnostics;   // Included for Timer usage

class Sierpinski
{
    static string[] colors = { "green", "yellow", "purple" };

    static string color = colors[0]; 
    static void CalcTriangle(Sage.Window win, int level, CPoint top_point, CPoint left_point, CPoint right_point,int depth)
    {
        // See if we should stop.
        if (level == 0) win.FillTriangle(top_point, right_point, left_point, color);
        else
        {
            // Find the edge midpoints.
            var left_mid = new CPoint((top_point.x + left_point.x) / 2.0f, (top_point.y + left_point.y) / 2.0f);
            var right_mid = new CPoint((top_point.x + right_point.x) / 2.0f, (top_point.y + right_point.y) / 2.0f);
            var bottom_mid = new CPoint((left_point.x + right_point.x) / 2.0f, (left_point.y + right_point.y) / 2.0f);
            // Recursively draw smaller triangles.
            if (depth == 0) color = colors[0];
            CalcTriangle(win, level - 1, top_point, left_mid, right_mid,depth+1);
            if (depth == 0) color = colors[1];
            CalcTriangle(win, level - 1, left_mid, left_point, bottom_mid,depth+1);
            if (depth == 0) color = colors[2];
            CalcTriangle(win, level - 1, right_mid, bottom_mid, right_point,depth+1);
        }
    }

    public static void SierpinskiTriangle()
    {
        var win = Sage.NewWindow();
        var input_box = Sage.DevInputBox("Enter Level (0-11)", kw.NumbersOnly() + kw.Range(0, 15));

        win.ClsRadial("darkblue,black");

        var center = win.GetWindowCenter();

        win.Write("{60}Enter level to draw Sierpinski Triangle.", kw.JustCenter());

        while (Sage.GetEvent())
        {
            if (input_box.ReturnPressed())
            {
                int iLevel = input_box.GetInteger();    // $$ Possibly get Value and GetValuef()

                win.Cls();
                win.Write($"{{40}}C# Sierpinski Triangle\n{{15}}{{cyan}}Level {iLevel}", kw.JustTopCenter() + kw.PadY(20));    // Write.. just and just text should be the same. 

                var p1 = new CPoint(0, -300);
                var p2 = new CPoint(500, 350);
                var p3 = new CPoint(-500, 350);


                Stopwatch sw = new Stopwatch(); 
                sw.Start();
                CalcTriangle(win, iLevel, p1 + center, p2 + center, p3 + center,0);
                sw.Stop();
                Console.WriteLine("Elapsed={0}", sw.Elapsed);
                input_box.ClearText();
            }
        }

        Sage.ExitButton();
    }


}

