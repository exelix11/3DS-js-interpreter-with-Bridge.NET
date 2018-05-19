# Nameless js interpreter
It's based on duktape and dukglue, with bindings to pp2d and a few custom libs, more info in JsApi.md
Make a folder called js in the root of the sd card and place your homebrews there.
This should work in citra.

With birdge.NET we can compile C# code to js, i used it in [Yata online](https://github.com/exelix11/Yata-online), this allows us to code homebrews in c# and run them on 3ds.
Unfortunately the 3DS is not powerful enough, running complex homebrews requires a lot of optimization, and C# code adds a non-insignificant overhead making it even more limited.
~~Maybe the next console will perform better ( ͡° ͜ʖ ͡°)~~

## Credits
This editor contains code or libraries from:
- [Duktape](http://duktape.org/)
- [Dukglue](https://github.com/Aloshi/dukglue)
- [pp2d](https://github.com/BernardoGiordano/PKSM/tree/master/source/pp2d)
- Everyone from the scene who worked on Ctrulib