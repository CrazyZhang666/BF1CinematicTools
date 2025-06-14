using CommunityToolkit.Mvvm.ComponentModel;

namespace BF1CinematicTools.Models;

public partial class ModModel : ObservableObject
{
    [ObservableProperty]
    private bool isCanRunGame;
}