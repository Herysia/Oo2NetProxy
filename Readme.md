# Oo2NetProxy

This is a small library to use as a proxy for calling oo2net_9_win64 Init and Decode from C# without issues.

This dll will store Oodle Shared and State object in his own memory space.

# Usage

```cs
//Declaration
[DllImport(@"lib\Oo2NetProxy.dll", EntryPoint = "#2", CharSet = CharSet.Unicode)]
private static extern int Oo2NetProxy_Init(byte[] path);

[DllImport(@"lib\Oo2NetProxy.dll", EntryPoint = "#1")]
private static extern unsafe int Oo2NetProxy_Decode(byte* inBuff, int inLen, byte* outBuff, int outLen);

//Init
Oo2NetProxy_Init(File.ReadAllBytes("PATH_TO_STATE/SHARED"));


//Decode
var outBuff = new byte[rawLen];
fixed (byte* outPtr = &outBuff[0])
fixed (byte* inPtr = &inBuff[0])
{
    Oo2NetProxy_Decode(inPtr, inLen, outPtr, rawLen);
}

```

# Disclaimer

This library isn't very generic, feel free to PR any changes that you think can improve it. But it's working
