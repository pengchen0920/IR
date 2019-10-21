from nltk.tokenize import RegexpTokenizer
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords
from collections import Counter
import math

def tokens(article_index):
    with open(str(article_index) + ".txt") as fp: 
        lines = fp.read().splitlines()
    text = ""
    text = " ".join(lines)
    
    # (1)Tokenization
    # remove the punctuations while making tokens
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

    return tokens_filtered

def tfidf_vector(article_index, word_list, dictionary):
    docu_tokens = tokens(article_index)
    term_count = 0
    vec_length_square = 0.0
    vec = []
    for word in sorted(set(docu_tokens)):
        if word in word_list:
            
            term_count += 1
            # calculate the square of the length the tfidf vector
            vec_length_square += (docu_tokens.count(word) * math.log10(N / dictionary[word]))**2
            vec.append([word_list.index(word) + 1, docu_tokens.count(word) * math.log10(N / dictionary[word])])
            
    return term_count, vec, vec_length_square**(0.5) # length it self

def tfidf_unit_vector_output(article_index, word_list, dictionary):
    term_count, vector, length = tfidf_vector(article_index, word_list, dictionary)
    for i in range(len(vector)):
        
        # devide every entry by the length of the vector to create unit vector
        vector[i][1] = (vector[i][1] / length)
        
    with open(str(article_index)+'_unit.txt', 'w') as file:
        file.write('%-5s\n' %term_count)
        file.write('%-5s\t%-15s\t\n' %('t_index', 'tfidf'))
        for t_index, tfidf in vector:
            file.write('%-5d\t%-15f\n' %(t_index, tfidf))

def tfidf_unit_vector(article_index, word_list, dictionary):
    term_count, vector, length = tfidf_vector(article_index, word_list, dictionary)
    for i in range(len(vector)):
        vector[i][1] = (vector[i][1] / length)
    return vector

def cosine_similarity(doc_x, doc_y):
    vec_x = tfidf_unit_vector(doc_x, word_list, dictionary)
    vec_y = tfidf_unit_vector(doc_y, word_list, dictionary)
    
    p_x = 0
    p_y = 0
    
    cosine_result = 0.0
    
    # only do inner poduct if two documents have the same t_index
    # else their inner product will be zero since tf of one document is zero
    while p_x < len(vec_x) and p_y < len(vec_y):
        if vec_x[p_x][0] == vec_y[p_y][0]:
            cosine_result += vec_x[p_x][1] * vec_y[p_y][1]
            p_x += 1
            p_y += 1
            
        elif vec_x[p_x][0] < vec_y[p_y][0]:
            p_x += 1
        else:
            p_y += 1
    return round(cosine_result, 5)

def hasNumbers(inputString):
    return any(char.isdigit() for char in inputString)


# corpus size
N = 1095

tokens_all = []
for i in range(1, N+1):
    tokens_all.extend(set(tokens(i)))

tokens_all.sort()

dictionary = {}
for word, df in Counter(tokens_all).items():
    # if df equals one, inner product will be zero while calculating cosine similarity
    # since it appears in only one document. So I will eliminate that term
    
    if df > 1 and len(word) > 1:
        if (hasNumbers(word)) or ('_' in word):

            # if the number can be recognized as a year, like 19xx or 20xx, then save it as a term        
            if  (len(word) is 4) and ((word[0] is '1' and word[1] is '9') or (word[0] is '2' and word[1] is '0')):
                dictionary[word] = df
        else:
            dictionary[word] = df


with open('IR_hw2_dictionary.txt', 'w') as file:
    file.write('%-5s\t%-8s\t%-4s\n' %('t_index', 'term', 'df'))
    
    for i,word, df in zip(range(1, len(dictionary)+1), dictionary.keys(), dictionary.values()): # t_index, term, df
        file.write('%-5d\t%-8s\t%-4d\n' %(i, word, df))

word_list = list(dictionary)
tfidf_unit_vector_output(1, word_list, dictionary)
