using CommunityToolkit.Mvvm.ComponentModel;

namespace BF1CinematicTools.Models;

public partial class MainModel : ObservableObject
{
    [ObservableProperty]
    private bool isNeedUpdate;
}