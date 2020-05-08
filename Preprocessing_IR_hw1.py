"""
若遇到SSL錯誤訊息，請輸入：


import nltk
import ssl

try:
    _create_unverified_https_context = ssl._create_unverified_context
except AttributeError:
    pass
else:
    ssl._create_default_https_context = _create_unverified_https_context

nltk.download()


(資料來源：https://stackoverflow.com/questions/41348621/ssl-error-downloading-nltk-data)
"""


from nltk.tokenize import RegexpTokenizer
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords

# 將 https://ceiba.ntu.edu.tw/course/35d27d/content/28.txt 中的文字轉存為 IR_hw1_input.txt
with open("IR_hw1_input.txt") as fp: 
    lines = fp.read().splitlines()

text = ""
text = " ".join(lines)


# (1)Tokenization
# 切割字的同時，清除標點符號
tokenizer = RegexpTokenizer(r'\w+')  
tokens = tokenizer.tokenize(text)


# (2)Lowercasing everything：
tokens_lower = [w.lower() for w in tokens]


# (3)Stemming using Porter’s algorithm
porter_stemmer = PorterStemmer()
tokens_stemmed = [porter_stemmer.stem(w) for w in tokens_lower]


# (4)Stopwords removal
stop_words = set(stopwords.words('english'))
tokens_filtered = []   
for w in tokens_stemmed: 
    if w not in stop_words: 
        tokens_filtered.append(w) 

# final output
with open('IR_hw1_output.txt', 'w') as file:
    for tokens in tokens_filtered:
        file.write('%s\n' % tokens)





