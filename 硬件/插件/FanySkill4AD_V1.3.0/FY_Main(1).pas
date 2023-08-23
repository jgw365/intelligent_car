{ **************************************************************************}
{   FY AD Tools : FY_Main                                    }
{   for AD19                                                                 }
{   version 1.0                                                             }
{                                                                           }
{   written by Fanyedu                                                         }
{   copyright @ 2019  Fanyedu CAD                                         }
{   Email : cad@fany-eda.com                                         }
{***************************************************************************}

procedure TFP_Main_Form.FP_CheckTextDirection_FindClick(Sender: TObject);
begin
     FY_CheckTextDirection;
end;

procedure TFP_Main_Form.Button4Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_Index_Sheet;
     Panel1.Caption := '          Altium Designer 辅助脚本';
     Statusbar1.Panels.Items[0].Text:='  All rights reserved: 凡亿教育 www.fanyedu.com';
end;

procedure TFP_Main_Form.FY_Main_ExitClick(Sender: TObject);
begin
     FP_Main_Form.Close;
end;

procedure TFP_Main_Form.SawpClick(Sender: TObject);
begin
     FY_ChooseAndSwapComponents;
end;


procedure TFP_Main_Form.SawpClick_Page(Sender: TObject);
begin
     PageCtr1.ActivePage := FY_Sawp_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.N111Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_CheckTextDirection_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.N21Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_CheckDuplicateComp_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.FP_Main_FormCreate(Sender: TObject);
var
     Board : IPCB_Board;
begin
     PageCtr1.ActivePage := FP_Index_Sheet;
     Panel1.Caption := '          Altium Designer 辅助脚本 ';
     Statusbar1.Panels.Items[0].Text:='  All rights reserved: 凡亿教育 www.fanyedu.com';
     Board := PCBServer.GetCurrentPCBBoard;
     If Board = Nil Then
     Begin
        ShowWarning('本程序只适用于 PCB 类型文档!');
        Exit;
        FP_Main_Form.Free;
     End;
end;

procedure TFP_Main_Form.FP_Main_Exit_ButtonClick(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_Index_Sheet;
     Panel1.Caption := '          Altium Designer 辅助脚本';
     Statusbar1.Panels.Items[0].Text:='  All rights reserved: 凡亿教育 www.fanyedu.com';
end;


procedure TFP_Main_Form.FP_CheckDuplicateComp_ButtonClick(Sender: TObject);
begin
     FY_CheckDuplicateComp;
end;

procedure TFP_Main_Form.N31Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_CheckTextCross_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.FP_CheckTextCross_Button1Click(Sender: TObject);
begin
     FP_CheckTextCross_Top(eMechanical16);
end;

procedure TFP_Main_Form.FP_CheckTextCross_Button2Click(Sender: TObject);
begin
     FP_CheckTextCross_Bottom(eMechanical16);
end;

procedure TFP_Main_Form.FP_CheckTextCross_Button3Click(Sender: TObject);
begin
     FP_CheckTextCross_Restore(eMechanical16);
end;


procedure TFP_Main_Form.N221Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_AssignNetsToPoly_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.FY_Shape_Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_AssignNetsToPoly_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.Button3Click(Sender: TObject);
begin
     FY_AssignNetsToPoly_GetPadNetName;
end;

procedure TFP_Main_Form.Button2Click(Sender: TObject);
begin
     FY_AssignPolyNet;
end;

procedure TFP_Main_Form.FY_DIFF_Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_AddDiffPair_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
     FY_CreateClasses;
end;

procedure TFP_Main_Form.Button6Click(Sender: TObject);
begin
     FY_AddDiffPair_GetPadNetName2;
end;

procedure TFP_Main_Form.Button5Click(Sender: TObject);
begin
     FY_AddDiffPair_GetPadNetName1;
end;

procedure TFP_Main_Form.Button8Click(Sender: TObject);
begin
     FY_AddDiffPair;
end;

procedure TFP_Main_Form.UpDown1ChangingEx(Sender: TObject; var AllowChange: Boolean; NewValue: Smallint; Direction: TUpDownDirection);
begin
     FP_Main_Form.Edit6.Text := IntToStr(NewValue);
end;

procedure TFP_Main_Form.Button9Click(Sender: TObject);
begin
     FP_Main_Form.Edit3.Text := '';
     FP_Main_Form.Edit4.Text := '';
end;

procedure TFP_Main_Form.N1Click(Sender: TObject);
begin
    PageCtr1.ActivePage := FP_SwapComps_Sheet;
    Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.Button10Click(Sender: TObject);
begin
   FP_SwapComps;
end;
procedure TFP_Main_Form.N1Pin1Click(Sender: TObject);
begin
    PageCtr1.ActivePage := FP_MoveCompByPinOne_Sheet;
    Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.Button12Click(Sender: TObject);
begin
   FP_MoveCompByPinOne;
end;

procedure TFP_Main_Form.N2Click(Sender: TObject);
begin
   PageCtr1.ActivePage := FP_MoveCompByOrigin_Sheet;
   Panel1.Caption := '功能:' + Sender.caption;
end;

procedure TFP_Main_Form.Button11Click(Sender: TObject);
begin
   FP_MoveCompByOrigin;
end;


procedure TFP_Main_Form.FY_NetClass_Click(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_Netclass_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
     FY_CreateNetClasses;
end;

procedure TFP_Main_Form.Button_CONFIRM_NET_Click(Sender: TObject);
begin
     FY_NetClass_GetPadNetName;
end;

procedure TFP_Main_Form.Button_NET_ADD_Click(Sender: TObject);
begin
     FY_AddToNetClass;
end;

procedure TFP_Main_Form.Button_Choose_Via(Sender: TObject);
begin
     FY_GetVia;
end;

procedure TFP_Main_Form.Button_Via_Add_Click(Sender: TObject);
begin
     FY_AddViaStep;
end;

procedure TFP_Main_Form.FY_AddViaClick(Sender: TObject);
begin
     PageCtr1.ActivePage := FP_AddVia_Sheet;
     Panel1.Caption := '功能:' + Sender.caption;
end;



