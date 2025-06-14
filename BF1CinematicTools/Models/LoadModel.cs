﻿using CommunityToolkit.Mvvm.ComponentModel;

namespace BF1CinematicTools.Models;

public partial class LoadModel : ObservableObject
{
    [ObservableProperty]
    private string bf1Path;

    [ObservableProperty]
    private bool isInitSuccess;
}