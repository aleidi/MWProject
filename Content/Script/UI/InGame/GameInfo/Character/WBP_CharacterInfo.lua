--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CharacterInfo_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:BindEvents()
end

function M:Construct()
    self:BindEvents()
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

---@param NewInfo FMWCharacterInfo
function M:SetCharacterInfo(NewInfo)
    -- set icon
    self.Head:SetBrushFromTexture(NewInfo.Head, false)
    
    self.Name:SetText(NewInfo.Name)
end

return M
