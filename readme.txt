�i��ɒP���ȁj�P��̑O��ɓ���̕����i��j��t�����A
�l�X�ȃp�^�[���̕�������쐬
�E�P��͘A�����邪�A�t�����镶���i��j�͘A�����Ȃ�
�E�ŏ��ɂ����t���Ȃ������i��j�ƍŌ�ɂ����t���Ȃ������i��j��ʂɎw��o����

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



���̂Ƃ���VS2015�p�A�������}���`�o�C�g��p
Linux BSD �ӂ��hoge�낤�Ǝv�����񂾂���UTF�ǂ����悤�ł��̂܂�
���̓t�@�C���͂P�̂�
�o�̓t�@�C���̎w��𖳎�����ƕW���o�͂��g��
�W���o�͂� Win �̎d�l�� CRLF�A-o �ŏo�͂���� LF

����̊ϑ��́A
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

�ŏo�Ă��� zzzzz.txt ���Q��

