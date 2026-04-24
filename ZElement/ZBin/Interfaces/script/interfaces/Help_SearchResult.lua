local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local SearchContent = SearchContent
local GameAI = GameAI

local datamember = 
{ 
	this = "Win_Help_SearchResult",
	m_SearchIndex = {},		--索引
	m_ClassIndex = {},      --按类别来建立小的索引
--	m_CatalogIndex = {},	--类别索引
	m_KeyWords = {}, 		--输入的关键字
	m_Result = {},	 		--最终的搜索结果	
	m_ResultSet = {},		--各个关键字对应的搜索的集合

	m_CurIndex = {}, 		--当前使用的索引，大索引或者是对应某个类别的小索引
	m_SelectedClass = 0,	--当前选中的搜索类别目前是12类

	m_ResultNum		=0,
}

Win_Help_SearchResult= DlgTemplate:new(datamember)


function Win_Help_SearchResult:Init()
    
    self:RegisterEvent("Btn_Search",self.OnCmdSearch);
    self:RegisterEvent("Btn_Close", self.OnCmdClose);
    self:RegisterEvent("Btn_UpInfo", self.OnCmdOpenLevelUp);
    self:RegisterEvent(WM_LBUTTONDOWN,  self.OnLButtonDown)
    self:RegisterEvent(WM_KEYDOWN,      self.OnKeyDown) 
	self:BuildIndex();
end

function Win_Help_SearchResult:OnCmdSearch()
	self:ExecuteSearch();
end

function Win_Help_SearchResult:OnCmdOpenLevelUp()
   DlgApi.ShowDialog("Win_Help_LvUp", true);  
end

function Win_Help_SearchResult:OnCmdClose()
   DlgApi.ShowDialog(self.this, false); 
end


function Win_Help_SearchResult:BuildIndex()
	for i, v in ipairs(SearchContent) do
    	if(not self.m_ClassIndex[v.class]) then
	        self.m_ClassIndex[v.class] = {};
		end
		for key, val in pairs(v.keys) do
		    if(not self.m_SearchIndex[val]) then
		        self.m_SearchIndex[val] = {};
			end
		   	table.insert(self.m_SearchIndex[val], i); 
			--建立小索引

			if(not self.m_ClassIndex[v.class][val]) then
			    self.m_ClassIndex[v.class][val] = {};
			end
			table.insert(self.m_ClassIndex[v.class][val], i);
		end
--		self.m_CatalogIndex[v.catalog] = i;	    
	end 
	self.m_CurIndex = self.m_SearchIndex   
end

function Win_Help_SearchResult:SplitKeyWords()
    local str = DlgApi.GetItemText(self.this, "DEFAULT_KeyWord");
    -- 在结果中搜索，则清除原来的关键字
    if (not DlgApi.IsItemChecked(self.this, "Chk_SearchAgain")) then
        self.m_KeyWords = {}
    end
    
    for word in string.gmatch(str, "%S+") do
--        if(#words < 3) then
    -- 去掉重复的关键字
    	if (self.m_KeyWords[word] == nil) then
	       	table.insert(self.m_KeyWords, word)
    	end
    end    
end

-- 选择当前使用的索引集合

function Win_Help_SearchResult:ChoseIndex()

    if self.m_SelectedClass >= 1 and self.m_SelectedClass <=12 and self.m_ClassIndex[self.m_SelectedClass] ~= nil then
        self.m_CurIndex = self.m_ClassIndex[self.m_SelectedClass]
    else
        self.m_CurIndex = self.m_SearchIndex
    end
    
end

function Win_Help_SearchResult:ExecuteSearch()
	self:SplitKeyWords()
	-- 关键词个数为0，直接返回

    if(#self.m_KeyWords == 0) then
       return;
    end
    
    self.m_ResultSet = {}
	local indexMask = {}
    self.m_Result = {}
    self.m_ResultNum = 0

    for i, v in pairs(self.m_KeyWords) do
        if(self.m_CurIndex[v] ~= nil) then
            self.m_ResultSet[v] = self.m_CurIndex[v]
        elseif(string.find(v, "*") ~= nil) then
        	self:PatternMatch(v)
--        	if(pattern ~= nil) then
--        	    indexSet[v] = self.m_CurIndex[pattern]
--    		end    
        else
            self:FuzzyMatch(v)
--            local similarKey = self:FuzzyMatch(v)
--            if( similarKey ~= nil) then
--	            indexSet[v] = self.m_CurIndex[similarKey]
--    		end
    	end     
    end
    
    local keyNum = 0
    for k, v in pairs(self.m_ResultSet) do
        keyNum = keyNum +1
    	for key, val in pairs(v) do
    	    if(not indexMask[val]) then
    	        indexMask[val] = 0;
    		end
    		indexMask[val] = indexMask[val] + 2^(keyNum-1);
    	end
    end

    local mask = 2^(keyNum) - 1
    for k, v in pairs(indexMask) do
        if( v == mask ) then
			table.insert(self.m_Result, k)
			self.m_ResultNum = self.m_ResultNum + 1
    	end
    end            
    
    self:InitCommandTree();
    if (self.m_ResultNum == 0) then
    	DlgApi.PopupMessageBox(0,"没有合适的结果", "请改变关键字，重新搜索")
    end
--    for i,v in ipairs(self.m_Result)do
--    	GameAI.SecretarySetURL(SearchContent[v].content);
--       DlgApi.AppendItemText(self.this, "SearchResult2", SearchContent[v].content) 
--    end
end

function Win_Help_SearchResult:FuzzyMatch(str)
	for k, v in pairs(self.m_CurIndex) do
        if( string.find(k, str) ~= nil)then
        
            for ke, ve in pairs(self.m_CurIndex[k]) do
                if(self.m_ResultSet[str] == nil) then
                    self.m_ResultSet[str] = {}
            	end
            	if(self.m_ResultSet[str][ve] == nil) then
               		table.insert(self.m_ResultSet[str], ve)
				end 
            end        
--          	return  k;
        end
    end
    return nil;
end

function Win_Help_SearchResult:PatternMatch(str)
    local _, count = string.gsub(str, "[^\128-\193]", "")
    local pattern = string.gsub(str, "*", "%%S+")
    for k, v in pairs(self.m_CurIndex) do
        local key = string.match(k, pattern)
        if(key ~= nil) then
            local _, len = string.gsub(key, "[^\128-\193]", "")
            if(len == count) then
                for ke, ve in pairs(self.m_CurIndex[k]) do
                    if(self.m_ResultSet[str] == nil) then
                        self.m_ResultSet[str] = {}
                	end
                	if(self.m_ResultSet[str][ve] == nil) then
                   		table.insert(self.m_ResultSet[str], ve)
    				end 
                end
--                return k;
    		end
		end
	end
end

-------------------------------------
--		windows message
-------------------------------------
function Win_Help_SearchResult:OnLButtonDown(item)
    if item == "Tv_SearchResult" then
        return self:UpdateCommandInfo()                    
    end
    for i =1, 12 do
        if item == "Rdo_"..i then
            self.m_SelectedClass = i
            break
        end
    end
    
    if item == "" then
		m_SelectedClass = 0       
    end
    return false
end

function Win_Help_SearchResult:OnKeyDown(msg, vkey)
    if vkey == VK_LEFT or vkey == VK_UP or vkey == VK_RIGHT or vkey == VK_DOWN then
        return self:UpdateCommandInfo()
    end
    return false
end

-------------------------------------
--		function called by others
-------------------------------------
function Win_Help_SearchResult:UpdateCommandInfo()
    local _, pos = DlgApi.GetTreeSelectItem(self.this, "Tv_SearchResult")
    if not pos then
        return false
    end
    
    local commandInfo = self:GetCommandInfo(pos)
    if commandInfo then
        GameAI.SecretarySetURL(SearchContent[commandInfo].content);
        return true
    end
--    if commandInfo then
--        DlgApi.SetItemText(self.this, "label_commandDesc", commandInfo.desc or "")
--        return true
--    else
--        DlgApi.SetItemText(self.this, "label_commandDesc", "")
--    end
    return false
end


function Win_Help_SearchResult:Align()
    local deltaX = -30 * DlgApi.GetWindowScale()
    DlgApi.SetDialogPosition(self.this, deltaX, 0, "panel_chatSetting", AUIDIALOG_ALIGN_OUTSIDE,
							 AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM)
end


function Win_Help_SearchResult:InitCommandTree()
    local treeInfo = {}    
    for i1, v1 in ipairs(self.m_Result) do
--        local folder = {v1.name, }
--        if #v1 > 0 then
--            for i2, v2 in ipairs(v1) do
--                table.insert(folder, {v2.name,})
--            end
--        end
        table.insert(treeInfo, {SearchContent[v1].title,})
    end
    DlgApi.SetTreeText(self.this, "Tv_SearchResult", treeInfo)
end

function Win_Help_SearchResult:GetCommandInfo(pos)
    if pos then
        local index = 1
        local item  = self.m_Result
        while pos[index] do
            item = item[pos[index] + 1]
            if not item then
                return
            end
            if not pos[index + 1] then
                return item
            end
            index = index + 1
        end
    end
end

function Win_Help_SearchResult:ShowDialog()
    -- 关闭帮助界面
    DlgApi.ShowDialog("Win_Help_Cyclo", false)
end

