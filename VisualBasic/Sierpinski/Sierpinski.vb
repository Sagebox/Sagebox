
Imports Sierpinski.Sagebox
Imports System.Drawing


Public Class Sierpinski

    Private Shared colors As String() = {"green", "yellow", "purple"}

    Private Shared color As String = colors(0)

    Private Shared Sub CalcTriangle(ByRef win As Sage.Window, level As Integer, top_point As CPoint, left_point As CPoint, right_point As CPoint, depth As Integer)
        '  See if we should stop.
        If level = 0 Then
            win.FillTriangle(top_point.pointF(), right_point.pointF(), left_point.pointF(), color)
        Else
            ' Find the edge midpoints.

            Dim left_mid = New CPoint((top_point.x + left_point.x) / 2.0F, (top_point.y + left_point.y) / 2.0F)
            Dim right_mid = New CPoint((top_point.x + right_point.x) / 2.0F, (top_point.y + right_point.y) / 2.0F)
            Dim bottom_mid = New CPoint((left_point.x + right_point.x) / 2.0F, (left_point.y + right_point.y) / 2.0F)

            ' Recursively draw smaller triangles.

            If depth = 0 Then color = colors(0)
            CalcTriangle(win, level - 1, top_point, left_mid, right_mid, depth + 1)
            If depth = 0 Then color = colors(1)
            CalcTriangle(win, level - 1, left_mid, left_point, bottom_mid, depth + 1)
            If depth = 0 Then color = colors(2)
            CalcTriangle(win, level - 1, right_mid, bottom_mid, right_point, depth + 1)
        End If
    End Sub

    Public Shared Sub SierpinskiTriangle()
        Dim win = Sage.NewWindow()
        win.ClsRadial("darkblue,black")
        Dim input_box = Sage.DevInputBox("Enter Level (0-11)", kw.NumbersOnly() + kw.Range(0, 11))

        Dim center = win.GetWindowCenter()

        win.Write("{60}Enter level to draw Sierpinski Triangle.", kw.Center())

        While Sage.GetEvent()
            if input_box.ReturnPressed() then
                Dim iLevel = input_box.GetInteger() 
            
                win.Cls()
                win.Write("{40}Visual Basic Sierpinski Triangle" + vbLf + "{15}{cyan}Level " & iLevel, kw.Just("topcenter") + kw.PadY(20))

                Dim p1 = new CPoint(0,-300) + center
                dim p2 = new CPoint(500,350) + center
                dim p3 = new CPoint(-500,350) + center

                CalcTriangle(win, iLevel, p1, p2, p3, 0)
                input_box.ClearText()
            End If
        End While

        Sage.ExitButton()
    End Sub
End Class
Module Program
    Sub Main()
        Sierpinski.SierpinskiTriangle()
    End Sub
End Module
