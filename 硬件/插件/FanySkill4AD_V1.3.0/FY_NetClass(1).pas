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
        If NetClass.MemberKind = eClassMemberKind_Net Then                      
        Begin
            If NetClass.Name = ANetClassName then                               
            Begin
                  NetObject	:= FP_GetNetObjectByName(Board, NetName);                         
                  If  NetObject <> Nil then                                     
                  Begin                       				   
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
       If NetClass.MemberKind = eClassMemberKind_Net Then                       
       Begin                                                                            
           FP_Main_Form.comboBoxClassList_NETCLASS.items.AddObject(NetClass.Name, i);   
           ClassList.Add(NetClass.Name);                                                
       End;
       NetClass := ClassIterator.NextPCBObject;                                
   End;
   Board.BoardIterator_Destroy(ClassIterator);                                  
End;




Function FY_NetClass_GetPadNetName();
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
                   FP_Main_Form.Edit_netname.Text := PadObject.Net.Name;
                   Exit;
               end
            else
               Begin
                   FP_Main_Form.Edit_netname.Text := '';
               End;

        Until (PadObject = Nil);

    Finally
        Pcbserver.PostProcess;
    End;
End;


Function FY_CreateNetClasses();  
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


Procedure FY_AddToNetClass();
Var
    NetObject1: IPCB_Net;
    Board     : IPCB_Board;
    DiffCount : Interger;
    DiffPrefix : String;
	NetClass      :  IPCB_ObjectClass;
	i             : Integer;
Begin

    Try
        Board := PCBServer.GetCurrentPCBBoard;
        If Not Assigned(Board) Then Exit;			

        NetObject1 := FP_GetNetObjectByName(Board,FP_Main_Form.Edit_netname.Text);
        

        If Not Assigned(NetObject1) Then
        begin
          FP_Main_Form.Edit_netname.Text := 'ÍøÂçÃûÎÞÐ§';
          Exit;
        end;
		
		i := 0;
		while (i < ClassList.Count) Do 
		Begin
		If ClassList.Strings[i] = FP_Main_Form.comboBoxClassList_NETCLASS.text then
			Break
		Else
			Inc(i); 
		End;
		
		
		
		if (i = ClassList.Count) then 
		Begin
			PCBServer.PreProcess;
			NetClass := PCBServer.PCBClassFactoryByClassMember(eClassMemberKind_Net);
			NetClass.SuperClass := False;
			If Not (FP_Main_Form.comboBoxClassList_NETCLASS.text = '') Then
			begin
				NetClass.Name := FP_Main_Form.comboBoxClassList_NETCLASS.text;
				Board.AddPCBObject(NetClass);
				ClassList.Add(NetClass.Name)
			end;
		End;
		FY_AddNetNamesToNetClass(FP_Main_Form.comboBoxClassList_NETCLASS.text, FP_Main_Form.Edit_netname.Text);				
    Finally
        Pcbserver.PostProcess;
    End;
End;










