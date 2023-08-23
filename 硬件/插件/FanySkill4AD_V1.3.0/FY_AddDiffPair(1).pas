{ **************************************************************************}
{   FY AD Tools : FY_AddDiffPair                                     }
{   for AD19                                                                 }
{   version 1.0                                                             }
{                                                                           }
{   written by Fanyedu                                                          }
{   copyright @ 2019  Fanyedu CAD                                         }
{   Email : cad@fany-eda.com                                          }
{***************************************************************************}
//add by loen   2019-9-11
Var
 ClassList     :  TStringList;

Procedure AddNetNamesToNetClass(ANetClassName : String);                        
Var
   Board         :  IPCB_Board;  
   BoardIterator :  IPCB_BoardIterator; 
   ASetOfObjects : TObjectSet;                                                  
   PCBObject     : IPCB_Object;                                                 
   Iterator      : IPCB_BoardIterator;   
   //ClassList     :  TStringList;                                                
   NetClass      :  IPCB_ObjectClass;                                           
   ClassIterator :  IPCB_BoardIterator;     
Begin
    ASetOfObjects := MkSet(eArcObject,eTrackObject, eViaObject, ePadObject, eRegionObject);       
    Iterator := Board.BoardIterator_Create;                                     
    Iterator.AddFilter_ObjectSet(ASetOfObjects);                                
    Iterator.AddFilter_LayerSet(AllLayers);                                     

    ClassIterator    := Board.BoardIterator_Create;                             
    ClassIterator.SetState_FilterAll;                                           
    ClassIterator.AddFilter_ObjectSet(MkSet(eClassObject));                    

    NetClass := ClassIterator.FirstPCBObject;                                   
    While NetClass <> Nil Do                                                    
    Begin
        If NetClass.MemberKind = eClassMemberKind_DifferentialPair Then                      
        Begin
            If NetClass.Name = ANetClassName then                               
            Begin
                  PCBObject := Iterator.FirstPCBObject;                         
                  While PCBObject <> Nil Do                                     
                  Begin
                       If ((PCBObject.Net <> Nil) && (PCBObject.Selected))  Then 
                            NetClass.AddMemberByName(PCBObject.Net.Name);       

                       PCBObject := Iterator.NextPCBObject;                     
                  End;
                  Break;                                                        
            End;
        End;
        NetClass := ClassIterator.NextPCBObject;                                
    End;
    Board.BoardIterator_Destroy(Iterator);                                      
    Board.BoardIterator_Destroy(ClassIterator);                                 
End;



Procedure FY_AddNetNamesToNetClass(ANetClassName : String, NetName : String);                        
Var
   Board         :  IPCB_Board;  
   BoardIterator :  IPCB_BoardIterator; 
   ASetOfObjects : TObjectSet;                                                  
   PCBObject     : IPCB_Object;                                                 
   Iterator      : IPCB_BoardIterator;   
   //ClassList     :  TStringList;                                                
   NetClass      :  IPCB_ObjectClass;                                           
   ClassIterator :  IPCB_BoardIterator;  
   NetObject : IPCB_Net;   
Begin
	Board := PCBServer.GetCurrentPCBBoard;
	If Not Assigned(Board) Then Exit;
	
    ASetOfObjects := MkSet(eArcObject,eTrackObject, eViaObject, ePadObject, eRegionObject);       
    Iterator := Board.BoardIterator_Create;                                     
    Iterator.AddFilter_ObjectSet(ASetOfObjects);                                
    Iterator.AddFilter_LayerSet(AllLayers);                                     

    ClassIterator    := Board.BoardIterator_Create;                             
    ClassIterator.SetState_FilterAll;                                           
    ClassIterator.AddFilter_ObjectSet(MkSet(eClassObject));                    

    NetClass := ClassIterator.FirstPCBObject;  
	
    While NetClass <> Nil Do                                                    
    Begin
        If NetClass.MemberKind = eClassMemberKind_DifferentialPair Then                      
        Begin
            If NetClass.Name = ANetClassName then                               
            Begin
                  NetObject	:= FP_GetNetObjectByName(Board, NetName);                         
                  If  NetObject <> Nil then                                     
                  Begin                       				   
				  //FP_Main_Form.Statusbar1.Panels.Items[0].Text:= 'NetClass.Name = ' + NetClass.Name ;
                  NetClass.AddMemberByName(NetName);                                                 
                  End;
                  Break;                                                        
            End;
        End;
        NetClass := ClassIterator.NextPCBObject;                                
    End;
    Board.BoardIterator_Destroy(Iterator);                                      
    Board.BoardIterator_Destroy(ClassIterator);                                 
End;




Procedure CollectNetClasses(Dummy); 
Var                                                                             
   Board         :  IPCB_Board;  
   BoardIterator :  IPCB_BoardIterator;                                        
   //ClassList     :  TStringList;                                                
   NetClass      :  IPCB_ObjectClass;                                           
   ClassIterator :  IPCB_BoardIterator;   
   i : Integer;                                                                
Begin
   Board := PCBServer.GetCurrentPCBBoard;
   If Not Assigned(Board) Then Exit;

   ClassIterator := Board.BoardIterator_Create;                                 
   ClassIterator.SetState_FilterAll;                                            
   ClassIterator.AddFilter_ObjectSet(MkSet(eClassObject));                      
   NetClass := ClassIterator.FirstPCBObject;    
   ClassList := TStringList.Create;   
   i := 0;                                                                      
   While NetClass <> Nil Do                                                     
   Begin
       Inc(i);                                                                  
       If NetClass.MemberKind = eClassMemberKind_DifferentialPair Then                       
       Begin                                                                            
           FP_Main_Form.comboBoxClassList.items.AddObject(NetClass.Name, i);   
           ClassList.Add(NetClass.Name);                                                
       End;
       NetClass := ClassIterator.NextPCBObject;                                
   End;
   Board.BoardIterator_Destroy(ClassIterator);                                  
End;



Function FY_CreateClasses();  
Var
    Board     : IPCB_Board; 
	//ClassList     :  TStringList;	
Begin
    Board := PCBServer.GetCurrentPCBBoard;                                      
    If Board = Nil Then Exit;                                                 

    ClassList := TStringList.Create;                                            

    CollectNetClasses(Nil);                                                     

    //formNetClassGenerator.ShowModal;                                            
End;

//add by loen   2019-9-11  end



Function FY_AddDiffPair_GetPadNetName1();
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
                   FP_Main_Form.Edit3.Text := PadObject.Net.Name;
                   Exit;
               end
            else
               Begin
                   FP_Main_Form.Edit3.Text := '';
               End;

        Until (PadObject = Nil);

    Finally
        Pcbserver.PostProcess;
    End;
End;

Function FY_AddDiffPair_GetPadNetName2();
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
                   FP_Main_Form.Edit4.Text := PadObject.Net.Name;
                   Exit;
               end
            else
               Begin
                   FP_Main_Form.Edit4.Text := '';
               End;

        Until (PadObject = Nil);

    Finally
        Pcbserver.PostProcess;
    End;
End;






Procedure FY_AddDiffPair();
Var
    DiffObject,TempDiffObject : IPCB_DifferentialPair;
    NetObject1,NetObject2 : IPCB_Net;
    Board     : IPCB_Board;
    DiffCount : Interger;
    DiffPrefix : String;
	NetClass      :  IPCB_ObjectClass;
	i             : Integer;
Begin
    If FP_Main_Form.Edit5.Text = '' Then
       begin
          FP_Main_Form.Edit5.Text := '差分对名称不能为空';
          Exit;
       end;
    Pcbserver.PreProcess;
    Try
        Board := PCBServer.GetCurrentPCBBoard;
        If Not Assigned(Board) Then Exit;
		
	

        NetObject1 := FP_GetNetObjectByName(Board,FP_Main_Form.Edit3.Text);
        NetObject2 := FP_GetNetObjectByName(Board,FP_Main_Form.Edit4.Text);
        TempDiffObject := FP_GetDiffObjectByName(Board,FP_Main_Form.Edit5.Text + FP_Main_Form.Edit6.Text);

        If Assigned(TempDiffObject) Then
        begin
          ShowInfo('差分对已经存在');
          //FP_Main_Form.UpDown1.Position := FP_Main_Form.UpDown1.Position + 1;
          Exit;
        end;

        If Not Assigned(NetObject1) Then
        begin
          FP_Main_Form.Edit3.Text := '网络名无效';
          Exit;
        end;

        If Not Assigned(NetObject2) Then
        begin
          FP_Main_Form.Edit4.Text := '网络名无效';
          Exit;
        end;

        DiffObject := PCBServer.PCBObjectFactory(eDifferentialPairObject, eNoDimension, eCreate_Default);
        DiffObject.Name := FP_Main_Form.Edit5.Text + FP_Main_Form.Edit6.Text;
        DiffObject.PositiveNet := NetObject1;
        DiffObject.NegativeNet := NetObject2;

        Board.AddPCBObject(DiffObject);
        FP_Main_Form.Statusbar1.Panels.Items[0].Text:= '差分对 <' + FP_Main_Form.Edit5.Text + FP_Main_Form.Edit6.Text + '> 添加成功';
        FP_Main_Form.UpDown1.Position := FP_Main_Form.UpDown1.Position + 1;
		
		
		
		i := 0;
		while (i < ClassList.Count) Do 
		Begin
		If ClassList.Strings[i] = FP_Main_Form.comboBoxClassList.text then
			Break
		Else
			Inc(i); 
		End;
		
		//FP_Main_Form.Statusbar1.Panels.Items[0].Text:= 'ClassList.Count = ' + FloatToStr(ClassList.Count) +' -----i= '  + FloatToStr(i);
		
		
		if (i = ClassList.Count) then 
		Begin
			PCBServer.PreProcess;
			NetClass := PCBServer.PCBClassFactoryByClassMember(eClassMemberKind_DifferentialPair);
			NetClass.SuperClass := False;
			If Not (FP_Main_Form.comboBoxClassList.text = '') Then
			begin
				NetClass.Name := FP_Main_Form.comboBoxClassList.text;
				Board.AddPCBObject(NetClass);
				ClassList.Add(NetClass.Name)
			end;
		End;
		FY_AddNetNamesToNetClass(FP_Main_Form.comboBoxClassList.text, DiffObject.Name);
		//FY_AddNetNamesToNetClass(FP_Main_Form.comboBoxClassList.text, FP_Main_Form.Edit4.Text);				
    Finally
        Pcbserver.PostProcess;
    End;
End;










