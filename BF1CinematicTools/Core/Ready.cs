using BF1CinematicTools.Helper;
using BF1CinematicTools.Utils;

namespace BF1CinematicTools.Core;

public static class Ready
{
    /// <summary>
    /// 运行核心功能
    /// </summary>
    public static void Run()
    {
        try
        {
            // 打开服务进程
            LoggerHelper.Info("正在启动服务进程...");
            ProcessHelper.OpenProcess(CoreUtil.File_Service_EADesktop, true);

            LoggerHelper.Info("正在启动 LSX 监听服务...");
            LSXTcpServer.Run();

            LoggerHelper.Info("正在启动 Local HTTP 监听服务...");
            LocalHttpServer.Run();

            LoggerHelper.Info("正在启动EA看门狗服务...");
            EaWatchDog.Run();

            // 检查EA App注册表
            RegistryHelper.CheckAndAddEaAppRegistryKey();
        }
        catch (Exception ex)
        {
            LoggerHelper.Error("运行核心功能发生异常", ex);
        }
    }

    /// <summary>
    /// 停止核心
    /// </summary>
    public static void Stop()
    {
        try
        {
            LoggerHelper.Info("正在停止 Local HTTP 监听服务...");
            LocalHttpServer.Stop();

            LoggerHelper.Info("正在停止 LSX 监听服务...");
            LSXTcpServer.Stop();

            LoggerHelper.Info("正在停止EA看门狗服务...");
            EaWatchDog.Stop();

            // 关闭服务进程
            CoreUtil.CloseServiceProcess();
        }
        catch (Exception ex)
        {
            LoggerHelper.Error("停止核心功能发生异常", ex);
        }
    }
}