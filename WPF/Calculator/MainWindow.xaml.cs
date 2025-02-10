using System.Data;
using System.Linq.Expressions;
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

namespace Calculator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private bool _calculateFinished = true;

        public bool CalculateFinished
        {
            get => _calculateFinished;
        }

        public MainWindow()
        {
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitializeNumberButtons();
            InitializeOperatorButtons();
        }

        private void InitializeNumberButtons()
        {
            for (int i = 1; i <= 9; i++)
            {
                Button btn = CreateNumberButton(i);
                ButtonStackPanel.Children.Add(btn);
            }
            ButtonStackPanel.Children.Add(CreateButton("."));
            ButtonStackPanel.Children.Add(CreateNumberButton(0));
            ButtonStackPanel.Children.Add(CreateButton("00"));
        }

        private void InitializeOperatorButtons()
        {
            OperatorStackPanel.Children.Add(CreateButton("+"));
            OperatorStackPanel.Children.Add(CreateButton("-"));
            OperatorStackPanel.Children.Add(CreateButton("*"));
            OperatorStackPanel.Children.Add(CreateButton("/"));
            OperatorStackPanel.Children.Add(CreateButton("="));
        }

        private Button CreateNumberButton(int number)
        {
            Button btn = new Button
            {
                Content = number.ToString(),
                Width = 50,
                Height = 50,
                Margin = new Thickness(5),
                Tag = number
            };
            btn.Click += Button_Click; // 绑定点击事件
            return btn;
        }

        private Button CreateButton(string text)
        {
            Button btn = new Button
            {
                Content = text,
                Width = 50,
                Height = 50,
                Margin = new Thickness(5),
                Tag = text
            };
            btn.Click += Button_Click; // 绑定点击事件
            return btn;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn)
            {
                if (btn.Tag is int)
                    NumberButton_Click(btn, e);
                else if (btn.Tag is string)
                    OperatorButton_Click(btn, e);
            }
        }

        private void NumberButton_Click(Button sender, RoutedEventArgs e)
        {
            if (_calculateFinished)
                ResultTextBlock.Text = (string)sender.Content;
            else
                ResultTextBlock.Text += (string)sender.Content;
            _calculateFinished = false;
        }

        private void OperatorButton_Click(Button sender, RoutedEventArgs e)
        {
            _calculateFinished = false;
            if (sender.Content.ToString() == "=")
            {
                ResultTextBlock.Text = new DataTable().Compute(ResultTextBlock.Text, null).ToString();
                _calculateFinished = true;
                return;
            }
            if (ResultTextBlock.Text == "00")
            {
                ResultTextBlock.Text += sender.Content;
                return;
            }
            if (IsLastCharNumber())
            {
                ResultTextBlock.Text += sender.Content;
                return;
            }
        }

        // 判断最后一位是否是数字
        private bool IsLastCharNumber()
        {
            return char.IsDigit(ResultTextBlock.Text[^1]);
        }
    }
}