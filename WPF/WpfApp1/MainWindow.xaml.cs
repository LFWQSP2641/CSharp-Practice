using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Collections.ObjectModel;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private string _inputText = string.Empty;
        private MainViewModel _mainViewModel = new();
        public string InputText
        {
            get => _inputText;
            set
            {
                if (_inputText != value)
                {
                    _inputText = value;
                    OnPropertyChanged();
                }
            }
        }
        public MainViewModel MainViewModel
        {
            get => _mainViewModel;
            set
            {
                if (_mainViewModel != value)
                {
                    _mainViewModel = value;
                    OnPropertyChanged();
                }
            }
        }

        public ICommand AddDataCommand { get; }
        public ICommand ClearDataCommand { get; }

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            AddDataCommand = new RelayCommand(AddData);
            ClearDataCommand = new RelayCommand(ClearData);
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        private void AddData()
        {
            MainViewModel.ViewModelDatas.Add(new ViewModelData { Data = InputText });
            InputText = string.Empty;
        }

        private void ClearData()
        {
            MainViewModel.ViewModelDatas.Clear();
        }
    }

    public class RelayCommand(Action execute, Func<bool>? canExecute = null) : ICommand
    {
        private readonly Action _execute = execute ?? throw new ArgumentNullException(nameof(execute));
        private readonly Func<bool>? _canExecute = canExecute;

        public bool CanExecute(object? parameter) => _canExecute?.Invoke() ?? true;

        public void Execute(object? parameter) => _execute();

        public event EventHandler? CanExecuteChanged
        {
            add => CommandManager.RequerySuggested += value;
            remove => CommandManager.RequerySuggested -= value;
        }
    }

    public class ViewModelData : INotifyPropertyChanged
    {
        private string _data = string.Empty;

        public string Data
        {
            get => _data;
            set
            {
                if (_data != value)
                {
                    _data = value;
                    OnPropertyChanged();
                }
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }

    public class MainViewModel
    {
        public ObservableCollection<ViewModelData> ViewModelDatas { get; } = [];
    }
}