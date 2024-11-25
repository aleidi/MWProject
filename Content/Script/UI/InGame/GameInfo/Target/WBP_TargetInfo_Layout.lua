--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_TargetInfo_Layout_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    local world = self:GetWorld()
    if world then
        local mwbs = UE.UWMGameplayUtility.GetBattleSubSystem(world)
        if mwbs then
            mwbs.OnTargetSelected:Add(self, M.OnTargetSelected)
            mwbs.OnTargetCancelSelected:Add(self, M.OnTargetCancelSelected)
        end
    end
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:OnTargetSelected(TargetInfo)
    self.Target:SetText(UE.UKismetTextLibrary.Conv_StringToText(TargetInfo.Name))
    self.Target:SetVisibity(UE.ESlateVisibility.HitTestInvisible)
end

function M:OnTargetCancelSelected()
    self.Target:SetVisibity(UE.ESlateVisibility.Hidden)
end

return M
