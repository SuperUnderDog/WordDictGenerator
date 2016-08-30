（主に単純な）単語の前後に特定の文字（列）を付加し、
様々なパターンの複合語を作成
・単語は連続するが、付加する文字（列）は連続しない
・最初にしか付かない文字（列）と最後にしか付かない文字（列）を別に指定出来る

WordDictGenerator

  usage: worddictgen [option]
  
  required
    -c <insert charactor(s) file>
  
  option
    -? -h                        // Help
    -o <output word list file>   // (default stdout)
    -i <input word list file>    // (default stdin)
    -cp <prefix charactor(s) file>
    -cs <surfix charactor(s) file>
    -omax <max out size(mega)>               // 1-? (default 4096)
    -wmin <min length>                       // 1-? (default 8)
    -wmax <max length>                       // 1-? (default 14)
    -in <max occurrences of input word>      // 1-? (default 1)
    -cn <max occurrences of insert char>     // 1-? (default 1)



今のところVS2015用、しかもマルチバイト専用
Linux BSD 辺りにhogeろうと思ったんだけどUTFどうしようでそのまま
入力ファイルは１つのみ
出力ファイルの指定を無視すると標準出力を使う
標準出力は Win の仕様上 CRLF、-o で出力すると LF

動作の観測は、
tchar.txt
>>>
-
+
>>>
tcharpre.txt
>>>
(
<
>>>
tcharsur.txt
>>>
)
>
>>>
tsrc.txt
>>>
HoGe
PiYo
>>>

WordDictGenerator.exe -in 2 -cn 3 -wmin 1 -c tchar.txt -cp tcharpre.txt -cs tcharsur.txt -i tsrc.txt > zzzzz.txt

で出てきた zzzzz.txt を参照

