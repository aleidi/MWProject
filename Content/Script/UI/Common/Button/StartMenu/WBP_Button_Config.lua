--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Button_Config_C
local M = UnLua.Class()

function M:OnInitialized()
    self.Overridden.OnInitialized(self)
end

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:BP_Onclicked()
    UE.UKismetSystemLibrary.PrintString(nil, "Set Config", true, false, UE.FLinearColor(1, 1, 0, 1), 10)
end

return M
