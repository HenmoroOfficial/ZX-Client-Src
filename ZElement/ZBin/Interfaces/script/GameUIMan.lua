--[[
--const set by c++
MB_OK           = 0
MB_OKCANCEL     = 1
MB_YESNO        = 4
]]--

GameUIMan = 
{
 this = 0,
 eventMap = {},
 messageboxCB = {}
}

local print = DebugPrint

function GameUIMan:MessageBox(name, text, type, r, g, b, a, CB)
   if CB then
        self.messageboxCB[name] = CB
   end
   return MessageBox(self.this, name, text, type, r, g, b, a)
end

function GameUIMan:OnMessageBox(name, bRet)
   local CB = self.messageboxCB[name]
   
   if CB and CB.OnEventMap then
        --print(name.." "..tostring(CB).." "..tostring(bRet).."\n")
        CB.OnEventMap(CB, name, bRet)
   end
end

function GameUIMan:RegisterEvent(event, handler)
    if  event and handler then
        self.eventMap[event] = handler
    else  -- need report ???
        print("lua register event:"..tostring(event).." handler:"..tostring(handler).." fail\n")
    end
end 

function GameUIMan:OnEventMap(event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
  local handler = self.eventMap[event]
  if handler and handler.OnEventMap then  
    handler.OnEventMap(handler, event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
  end  
end

