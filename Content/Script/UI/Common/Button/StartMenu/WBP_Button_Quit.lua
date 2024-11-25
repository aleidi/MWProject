--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Button_Quit_C
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

function M:BP_OnClicked()
    UE.UKismetSystemLibrary.PrintString(nil, "Quit Game", true, false, UE.FLinearColor(1, 1, 0, 1), 10)
    UE.UKismetSystemLibrary.QuitGame(self, nil, UE.EQuitPreference.Quit, false)
end

return M
