#include <fstream>
#include "tools/unicode_string_convert.h"
#include "tools/text.h"

int main() {

    std::ofstream file("./test.txt");
    std::u32string text=U"ھەممە ئادەم تۇغۇلۇشىدىنلا ئەركىن، ئىززەت-ھۆرمەت ۋە ھوقۇقتا باب-باراۋەر بولۇپ تۇغۇلغان. ئۇلار ئەقىلگە ۋە ۋىجدانغا ئىگە ھەمدە بىر-بىرىگە قېرىنداشلىق مۇناسىۋىتىگە خاس روھ بىلەن مۇئامىلە قىلىشى كېرەك.";
    file <<"text:"<<utf32_to_utf8(text)<<std::endl;
    file <<"text:"<<utf32_to_utf8(string_norm_reverse(text,1))<<std::endl;


    file.close();

    return 0;
}