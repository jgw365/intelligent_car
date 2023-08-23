{ **************************************************************************}
{   FY AD Tools : FY_Function                                     }
{   for AD19                                                                 }
{   version 1.0                                                             }
{                                                                           }
{   written by Fanyedu                                                        }
{   copyright @ 2019  Fanyedu CAD                                         }
{   Email : cad@fany-eda.com                                          }
{***************************************************************************}
Procedure FP_RemoveObjectByLayer(AObjectSet : TObjectSet ; ALayerSet : TLayerSet);
var
    CurrentPCBBoard : IPCB_Board;
    Iterator        : IPCB_BoardIterator;
    Track           : IPCB_Track;
    OldTrack        : IPCB_Track;
Begin
    CurrentPCBBoard := PCBServer.GetCurrentPCBBoard;
    If CurrentPCBBoard = Nil Then Exit;

    Iterator := CurrentPCBBoard.BoardIterator_Create;
    If Iterator = Nil Then Exit;
    Iterator.AddFilter_ObjectSet(AObjectSet);
    Iterator.AddFilter_LayerSet(ALayerSet);
    PCBServer.PreProcess;
    Try
        Track := Iterator.FirstPCBObject;
        While Track <> Nil Do
        Begin
            OldTrack := Track;
            Track := Iterator.NextPCBObject;
            CurrentPCBBoard.RemovePCBObject(OldTrack);
        End;
    Finally
        CurrentPCBBoard.BoardIterator_Destroy(Iterator);
    End;
End;

Function FP_GetNetObjectByName(ABoard :IPCB_Board ; Aname:String);
Var
    Net         : IPCB_Net;
    Iterator    : IPCB_BoardIterator;
Begin
    Iterator := ABoard.BoardIterator_Create;
    Iterator.AddFilter_ObjectSet(MkSet(eNetObject));
    Iterator.AddFilter_LayerSet(AllLayers);
    Iterator.AddFilter_Method(eProcessAll);

    Net := Iterator.FirstPCBObject;
    While (Net <> Nil) Do
    Begin
        If Net.Name = Aname Then
        Begin
            Result := Net;
            Break;
        End;
        Net := Iterator.NextPCBObject;
    End;
    ABoard.BoardIterator_Destroy(Iterator);
End;

Function FP_GetDiffObjectByName(ABoard :IPCB_Board ; Aname:String);
Var
    DiffObject  : IPCB_DifferentialPair;
    Iterator    : IPCB_BoardIterator;
Begin
    Iterator := ABoard.BoardIterator_Create;
    Iterator.AddFilter_ObjectSet(MkSet(eDifferentialPairObject));
    Iterator.AddFilter_LayerSet(AllLayers);
    Iterator.AddFilter_Method(eProcessAll);

    DiffObject := Iterator.FirstPCBObject;
    While (DiffObject <> Nil) Do
    Begin
        If DiffObject.Name = Aname Then
        Begin
            Result := DiffObject;
            Break;
        End;
        DiffObject := Iterator.NextPCBObject;
    End;
    ABoard.BoardIterator_Destroy(Iterator);
End;

Function FP_GetCompObjectByName(ABoard :IPCB_Board ; Aname:String);
Var
    Comp        : IPCB_Component;
    Iterator    : IPCB_BoardIterator;
Begin
    Iterator := ABoard.BoardIterator_Create;
    Iterator.AddFilter_ObjectSet(MkSet(eComponentObject));
    Iterator.AddFilter_LayerSet(AllLayers);
    Iterator.AddFilter_Method(eProcessAll);

    Comp := Iterator.FirstPCBObject;
    While (Comp <> Nil) Do
    Begin
        If Comp.Name.Text = Aname Then
        Begin
            Result := Comp;
            Break;
        End;
        Comp := Iterator.NextPCBObject;
    End;
    ABoard.BoardIterator_Destroy(Iterator);
End;

