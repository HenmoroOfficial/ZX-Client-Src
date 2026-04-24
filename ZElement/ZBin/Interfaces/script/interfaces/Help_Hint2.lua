local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local SecretarySystemRemind = SecretarySystemRemind


local datamember = 
{
	this = "Win_Help_Hint2",
	m_curEvent = 0,
	m_curMsgIndex = 0,
	m_counter = 0,
	m_bUsed  = false,
	
}


Win_Help_Hint2= DlgTemplate:new(datamember)


function Win_Help_Hint2:Init()
    
    self:RegisterEvent("Btn_Help", self.OnCmdToMain);
	self:RegisterEvent("Btn_Close", self.OnCmdClose);
    
end
    
function Win_Help_Hint2:OnCmdToMain()
	DlgApi.ShowDialog("Win_Help_Fairy", true);   
end
	
function Win_Help_Hint2:OnCmdClose()
	DlgApi.ShowDialog(self.this, false);
end


function Win_Help_Hint2:Tick(dwDeltaTime)
    self.m_counter  = self.m_counter + dwDeltaTime;
    if( self.m_counter > 20000 ) then
        self.m_counter = 0;
        self.m_bUsed = false;
        DlgApi.ShowDialog(self.this, false);
	end
end

function Win_Help_Hint2:IsUsed()
    if(self.m_bUsed) then
        return true;
	end
	return false
end
