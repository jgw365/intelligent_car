{ **************************************************************************}
{   FY AD Tools : FY_AddDiffPair                                     }
{   for AD19                                                                 }
{   version 1.0                                                             }
{                                                                           }
{   written by Fanyedu                                                          }
{   copyright @ 2019  Fanyedu CAD                                         }
{   Email : cad@fany-eda.com                                          }
{***************************************************************************}

Var
Via_x : String;
Via_y : String;
Via_hole : String;
Via_pad : String;
ViaObject : IPCB_Via ;






Function FY_AddOneVia();
Var
    Board    : IPCB_Board;
    BR       : TCoordRect;
    Sheet    : IPCB_Sheet;
    Via      : IPCB_Via;
    PadCache : TPadCache;
	NetObject : IPCB_Net;
	Via_Step : String;
	Via_Keep_NetName : String;
	Via_Dir  : Integer;
	
Begin
    // Grab the board interface representing the current PCB document in DXP.
    Board := PCBServer.GetCurrentPCBBoard;

    // If the board interface doesnt exist (no PCB document) then exit.
    If Board = Nil Then Exit;
    
    // Initialize the systems in the PCB Editor.
    PCBServer.PreProcess;

    Sheet := Board.PCBSheet;   
	
	If Not Assigned(ViaObject) Then Exit;
	
    Via  := PCBServer.PCBObjectFactory(eViaObject, eNoDimension, eCreate_Default);
	

	Via_Step :=	FP_Main_Form.Edit_Via_Step.Text;
	
	
	Case FP_Main_Form.Radio_NetName.ItemIndex of
        0 : Via_Keep_NetName := '0';
        1 : Via_Keep_NetName := '1';
    End;
	
	Case FP_Main_Form.Radio_Via_Direction.ItemIndex of
        0 : Via_Dir := 0;
        1 : Via_Dir := 1;
		2 : Via_Dir := 2;
        3 : Via_Dir := 3;
    End;

    // obtain the bottom left coordinates of the board outline
    BR := Board.BoardOutline.BoundingRectangle;
    
				   
    Via.Size      := StrToInt(Via_pad);
    Via.HoleSize  := StrToInt(Via_hole);


    Via.LowLayer  := eTopLayer;
    Via.HighLayer := eBottomLayer;
	NetObject := FP_GetNetObjectByName(Board,FP_Main_Form.Edit_VIA_net.Text);

	
	If Via_Keep_NetName = '0' Then
       begin
          Via.Net  := NetObject;
		  Via_Keep_NetName := '1';
       end;

	
    // Set up Cache info for Via
    // which consists mainly solder mask, paste mask and power plane values from design rules
    //Padcache                           := Via.GetState_Cache;
   // Padcache.ReliefAirGap              := MilsToCoord(11);
   // Padcache.PowerPlaneReliefExpansion := MilsToCoord(11);
   // Padcache.PowerPlaneClearance       := MilsToCoord(11);
   // Padcache.ReliefConductorWidth      := MilsToCoord(11);
   // Padcache.SolderMaskExpansion       := MilsToCoord(11);
   // Padcache.SolderMaskExpansionValid  := eCacheManual;
   // Padcache.PasteMaskExpansion        := MilsToCoord(11);
   // Padcache.PasteMaskExpansionValid   := eCacheManual;

    // Assign the new pad cache to the via 
   // Via.SetState_Cache                 := Padcache;

    // Put the new Via object on the board
	
	
	
		If Via_Dir = 0 Then
		begin
			Via.x := StrToInt(Via_x);
			Via.y := StrToInt(Via_y)+ (StrToFloat(Via_Step)*10000);
		end;
		
		If Via_Dir = 1 Then
		begin
			Via.x := StrToInt(Via_x);
			Via.y := StrToInt(Via_y) - (StrToFloat(Via_Step)*10000);
		end;
		
		If Via_Dir = 2 Then
		begin
			Via.x := StrToInt(Via_x) + (StrToFloat(Via_Step)*10000);
			Via.y := StrToInt(Via_y);
		end;
		
		If Via_Dir = 3 Then
		begin
			Via.x := StrToInt(Via_x) - (StrToFloat(Via_Step)*10000);
			Via.y := StrToInt(Via_y);
		end;
			
		
		
		
		Board.AddPCBObject(Via);	
		Via_x := FloatToStr(Via.x);
		Via_y := FloatToStr(Via.y); 

		// Update the Undo System in DXP that a new VIa object has been added to the board
		PCBServer.SendMessageToRobots(Board  .I_ObjectAddress, c_Broadcast, PCBM_BoardRegisteration, Via.I_ObjectAddress);
		       

	// Finalize the systems in the PCB Editor.
	PCBServer.PostProcess;      
    // Refresh PCB screen
    //Client.SendMessage('PCB:Zoom', 'Action=Redraw' , 255, Client.CurrentView);
End;


Function FY_AddViaStep();
Var
	Via_QTY : Integer;
	i        : Integer;
Begin
	If FP_Main_Form.Edit_Via_Amount.Text = '' Then
       begin
          FP_Main_Form.Edit_Via_Amount.Text := '1';
          Exit;
       end;
	   
	Via_QTY  :=	StrToInt(FP_Main_Form.Edit_Via_Amount.Text);
	
	for i:=1 to Via_QTY do
	begin
		FY_AddOneVia();
	end;
	
End;






Function FY_GetVia();
Var
    Board     : IPCB_Board;
    
Begin
    Pcbserver.PreProcess;

    Try
        Board := PCBServer.GetCurrentPCBBoard;
        If Not Assigned(Board) Then Exit;
        //Get Pad Net
        Repeat
            ViaObject := Board.GetObjectAtCursor(MkSet(eViaObject), AllLayers, 'Choose a pad');
            If Not Assigned(ViaObject) Then Exit;
            If ViaObject.Net <> Nil Then
               Begin
                   FP_Main_Form.Edit_VIA_net.Text := ViaObject.Net.Name;
				   Via_x := FloatToStr(ViaObject.x);
				   Via_y := FloatToStr(ViaObject.y);
				   Via_hole := FloatToStr(ViaObject.HoleSize);
				   Via_pad := FloatToStr(ViaObject.Size);
                   Exit;
               end
            else
               Begin
                   FP_Main_Form.Edit_VIA_net.Text := '';
               End;

        Until (ViaObject = Nil);

    Finally
        Pcbserver.PostProcess;
    End;
End;

