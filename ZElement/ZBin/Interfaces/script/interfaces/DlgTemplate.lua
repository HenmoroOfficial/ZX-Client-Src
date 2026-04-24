--[[
  description:   the template for all dialogs.
  author:      Wang Kuiwu (wangkuiwu@world2.cn)
  Copyright (c) , All Rights Reserved.
  warning:   Please do NOT modify this file.  Contact me if any question.
  警告   ：  请不要修改本文件。如有问题请与我联系。
  说明 1. 对于windows message的响应函数，必须有返回值
		  true表示响应了该message，false表示没有。
]]--



-----------------global func--------------
-- can make cpp define the DlgTemplate method  directly, not in another table,
-- Maybe better? NOT sure.

local DlgApi = DlgApi      --from c





-----------------template implementation--------------
DlgTemplate =
{
this = 0,
eventMap = {}
}


-------------------create instance--------------------------
function  DlgTemplate:new(datamember)
  local inst = CreateObj(self, datamember)
  return inst
end

function DlgTemplate:RegisterEvent(event, handler)
    if  event and handler then
        self.eventMap[event] = handler
    else  -- need report ???
        
    end
end 

function DlgTemplate:UnRegisterEvent(event)
    self.eventMap[event] = nil    
end 


function DlgTemplate:OnEventMap( event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
    local handler = self.eventMap[event];
    if not handler then
    	return false;
    end
	local ret = handler(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); 
	if ret ~= nil then
		return ret;
    else
    	return true;  --handler default return true
    end
end 

function DlgTemplate:OnCancel()
      DlgApi.ShowDialog(self.this, false)
end

function DlgTemplate:OnMessageBox(name, bRet)
end

-----------------overload method----------------------
function DlgTemplate:Init()
end



function DlgTemplate:ShowDialog()
end

function DlgTemplate:HideDialog()
end


function DlgTemplate:Release()
end

function DlgTemplate:IsTick()
   return (self.Tick ~= nil);
end

function DlgTemplate:IsRender()
 	 return (self.Render ~= nil);
end

