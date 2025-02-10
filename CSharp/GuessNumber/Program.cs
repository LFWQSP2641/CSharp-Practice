int number = new Random().Next(1, 100);

Console.WriteLine("猜数字（输入 Q 退出）");
Console.Write("请输入一个数字：");

int guess = 0;
while (true)
{
    string? input = Console.ReadLine();
    while (input == null)
    {
        Console.WriteLine("输入不能为空");
        input = Console.ReadLine();
    }
    if (input.Equals("q", StringComparison.OrdinalIgnoreCase))
    {
        return;
    }
    if (!int.TryParse(input, out guess))
    {
        Console.WriteLine("请输入有效的数字：");
        continue;
    }
    if (guess > number)
    {
        Console.WriteLine("猜大了");
    }
    else if (guess < number)
    {
        Console.WriteLine("猜小了");
    }
    else
    {
        Console.WriteLine("猜对了");
        break;
    }
    Console.Write("请再次输入一个数字：");
}
