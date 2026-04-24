--[[
UIMan模板，修改前请与我联系
author: wang kuiwu
说明 1. 对于windows message的响应函数，必须有返回值
		true表示响应了该message，false表示没有。
]]--

local DlgApi = DlgApi      --from c
local Insert = table.insert;

UIManTemplate = 
{
 this = 0,
 eventMap = {},
 messageboxCB = {}
}

function  UIManTemplate:new(datamember)
	self:RegisterEvent(LEVT_ERROR_MSG, self.OnErrorMsg)
  return CreateObj(self, datamember)
end

function UIManTemplate:AddMessageboxCB(name, CB)
		if type(CB) == "function" then
			  --self function
		 	  self.messageboxCB[name] = CB; 
		elseif type(CB) == "table" then
			--dialog messagebox handler
		   if self.messageboxCB[name] then
	   	   Insert(self.messageboxCB[name], CB);
	   	 else
	   		 self.messageboxCB[name] = {CB};
	  	 end	
	  end	 
end

function UIManTemplate:MessageBox(mtype, name, text, CB)
   if CB then
        self:AddMessageboxCB(name, CB);
   end
   return DlgApi.PopupMessageBox (mtype, name, text);
end


function UIManTemplate:OnMessageBox(name, bRet)
   local handler = self.messageboxCB[name];
	 
	 if not handler then
	 		return;
	 end	   
	 
   if type(handler) == "function" then
  	  handler(self, name, bRet);
   elseif type(handler) == "table" then
  	  local  v;
   		for  _, v in ipairs(handler) do
   			  v:OnMessageBox(name, bRet);
   		end	    
  end
end

function UIManTemplate:AddEventCB(name, CB)
		if type(CB) == "function" then
			  --self function
		 	  self.eventMap[name] = CB; 
		elseif type(CB) == "table" then
			--dialog event maper
	     if self.eventMap[name] then
	    	 Insert(self.eventMap[name], CB);
	   	 else
	   		 self.eventMap[name] = {CB};
	  	 end	
	  end	 
end

function UIManTemplate:RegisterEvent(event, handler)
    if  event and handler then
    		self:AddEventCB(event, handler);
    end
end 

function UIManTemplate:OnEventMap(event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
  local handler = self.eventMap[event];
  
  if not handler then
  	--LogPrint("no event hanler "..tostring(self.this).." "..tostring(event).."\n");
  	return false;
  end
  
  if type(handler) == "function" then
  	return  handler(self, event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
  elseif type(handler) == "table" then
  	  local  v;
   		for  _, v in ipairs(handler) do
   			  if v:OnEventMap(event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) then
   			  	  return true;
   			  end
   		end	    
  end
  return false;
 
end

function UIManTemplate:OnErrorMsg(event, errCode)
	local msg = ERROR_MSG_TABLE[errCode]
	if msg == nil then
		msg = "unknown error code"
	end
	self:MessageBox(MB_OK, "", msg)
end

function UIManTemplate:IsTick()
   return (self.Tick ~= nil);
end

function UIManTemplate:Init()
end

function UIManTemplate:Release()
end

function UIManTemplate:ResizeWindows()
end