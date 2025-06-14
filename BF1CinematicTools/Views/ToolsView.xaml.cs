using BF1CinematicTools.Helper;
using BF1CinematicTools.Utils;
using CommunityToolkit.Mvvm.Input;
using System.Threading.Tasks;

namespace BF1CinematicTools.Views;

/// <summary>
/// ToolsView.xaml 的交互逻辑
/// </summary>
public partial class ToolsView : UserControl
{
    public ToolsView()
    {
        InitializeComponent();
    }

    [RelayCommand]
    private async Task ClearCinematicData()
    {
        try
        {
            if (ProcessHelper.IsAppRun(CoreUtil.Name_BF1))
            {
                NotifierHelper.Warning("战地1正在运行，请关闭后再执行清理电影工具数据操作");
                return;
            }

            var cinematicDir = Path.Combine(Globals.BF1InstallDir, "Cinematic Tools");
            if (Directory.Exists(cinematicDir))
                await FileHelper.DeleteDirectoryAsync(cinematicDir);

            var imguiIniFile = Path.Combine(Globals.BF1InstallDir, "imgui.ini");
            if (File.Exists(imguiIniFile))
                await FileHelper.DeleteFileAsync(imguiIniFile);

            NotifierHelper.Success("执行电影工具数据操作成功");
        }
        catch (Exception ex)
        {
            LoggerHelper.Error($"清理电影工具数据发生异常", ex);
        }
    }

    [RelayCommand]
    private void ClearModData()
    {
        try
        {
            if (ProcessHelper.IsAppRun(CoreUtil.Name_BF1))
            {
                NotifierHelper.Warning("战地1正在运行，请关闭后再执行清理Mod数据操作");
                return;
            }

            if (ProcessHelper.IsAppRun(CoreUtil.Name_FrostyModManager))
            {
                NotifierHelper.Warning("FrostyModManager正在运行，请关闭后再执行清理Mod数据操作");
                return;
            }

            var modDataDir = Path.Combine(Globals.BF1InstallDir, "ModData");
            if (!Directory.Exists(modDataDir))
            {
                NotifierHelper.Warning("未发现战地1Mod数据文件夹，操作取消");
                return;
            }

            FileHelper.ClearDirectory(modDataDir);
            NotifierHelper.Success("执行清理Mod数据操作成功");
        }
        catch (Exception ex)
        {
            LoggerHelper.Error($"清理Mod数据发生异常", ex);
        }
    }

    [RelayCommand]
    private void KillBf1Process()
    {
        MainWindow.MainWinInstance.IsShowMaskLayer = true;
        if (MessageBox.Show("你确定要结束《战地1》进程吗？",
            "警告", MessageBoxButton.OKCancel, MessageBoxImage.Warning) == MessageBoxResult.OK)
        {
            ProcessHelper.CloseProcess(CoreUtil.Name_BF1);
            ProcessHelper.CloseProcess(CoreUtil.Name_FrostyModManager);
            ProcessHelper.CloseProcess(CoreUtil.Name_MarneLauncher);
        }
        MainWindow.MainWinInstance.IsShowMaskLayer = false;
    }

    [RelayCommand]
    private void ClearBf1Memory()
    {
        MainWindow.MainWinInstance.IsShowMaskLayer = true;
        if (MessageBox.Show("你确定要清理《战地1》内存吗？仅推荐在游戏占用过多内存时使用",
            "警告", MessageBoxButton.OKCancel, MessageBoxImage.Warning) == MessageBoxResult.OK)
        {
            ProcessHelper.ClearProcessMemory(CoreUtil.Name_BF1);
        }
        MainWindow.MainWinInstance.IsShowMaskLayer = false;
    }

    [RelayCommand]
    private void OpenIPOverrideWeb()
    {
        ProcessHelper.OpenLink("https://marne.io/api/override.php");
    }
}