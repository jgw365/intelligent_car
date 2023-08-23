{ **************************************************************************}
{   FY AD Tools : FY_AssignNetsToPoly                                     }
{   for AD19                                                                 }
{   version 1.0                                                             }
{                                                                           }
{   written by Fanyedu                                                          }
{   copyright @ 2019  Fanyedu CAD                                         }
{   Email : cad@fany-eda.com                                          }
{***************************************************************************}

Function FY_AssignNetsToPoly_GetPadNetName();
Var
    Board     : IPCB_Board;
    PadObject : IPCB_Pad ;
Begin
    Pcbserver.PreProcess;

    Try
        Board := PCBServer.GetCurrentPCBBoard;
        If Not Assigned(Board) Then Exit;
        //Get Pad Net
        Repeat
            PadObject := Board.GetObjectAtCursor(MkSet(ePadObject), AllLayers, 'Choose a pad');
            If Not Assigned(PadObject) Then Exit;
            If PadObject.Net <> Nil Then
               Begin
                   FP_Main_Form.Edit2.Text := PadObject.Net.Name;
                   Exit;
               end
            else
               Begin
                   FP_Main_Form.Edit2.Text := '';
               End;
        Until (PadObject = Nil);

    Finally
        Pcbserver.PostProcess;
    End;
End;

Procedure FY_AssignPolyNet();
Var
    Board     : IPCB_Board;
    PolyObject: IPCB_Polygon;
    NetObject : IPCB_Net;
Begin
    Pcbserver.PreProcess;
    Try
        Board := PCBServer.GetCurrentPCBBoard;
        If Not Assigned(Board) Then Exit;
        //Get Net
        NetObject := FP_GetNetObjectByName(Board,FP_Main_Form.Edit2.Text);
        If Not Assigned(NetObject) Then
           Exit
        Else
            Begin
                //Get Poly
                Repeat
                    PolyObject := Board.GetObjectAtCursor(MkSet(ePolyObject), AllLayers, 'Choose a Polygon');
                    If Not Assigned(PolyObject) Then Exit;
                    PolyObject.Net :=  NetObject;
                    PolyObject.Rebuild;
                    PolyObject.Selected := True;
                    If PolyObject.Net <> Nil Then FP_Main_Form.Edit1.Text := PolyObject.Net.Name;
                    Exit;
                Until (PolyObject = Nil);
            End;

    Finally
        Pcbserver.PostProcess;
        Client.SendMessage('PCB:Zoom', 'Action=Redraw', 255, Client.CurrentView);
    End;
End;
