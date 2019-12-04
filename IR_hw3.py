from nltk.tokenize import RegexpTokenizer
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords
from collections import Counter
import pandas as pd
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

def hasNumbers(inputString):
    return any(char.isdigit() for char in inputString)


# ## Training documents index --> train_nums

with open("train_num.txt") as train_num: 
    train_nums = train_num.read().splitlines()

for i in range(13):
    train_nums[i] = train_nums[i].split()
    train_nums[i].pop(0)
# print(train_nums)

# number of training data class
class_num = 13
# number of documents in each class
class_count = 15
# training corpus size
N_train = 13*15
N_all = 1095
N_test = N_all - N_train


# ## Feature Selection Preprocessing

train_tokens = []
train_terms = []

for i in range(class_num):
    train_tokens.append([])
    train_terms.append([])

for i in range(class_num):
    for j in range(class_count):
        train_tokens[i].append(set(tokens(train_nums[i][j])))

# train_terms[class i][jth article]
for i in range(class_num):
    for j in range(class_count):
        train_terms[i].append(list(set(train_tokens[i][j])))
        train_terms[i][j].sort()
# print(train_terms)

# delete '_' and too short term in train_terms
for i in range(class_num):
    for j in range(class_count):
        for k in range(len(train_terms[i][j])-1, -1, -1):
            if ('_' in train_terms[i][j][k]) or  len(train_terms[i][j][k])==1 :
                delete = train_terms[i][j].pop(k)
                # print(delete)
# print(train_terms)


# ## Feature Selection Function (Likelihood method) 

def feature_selection(class_num = 13, class_count = 15, train_terms = train_terms):
    
    # feature_dictionary[ith class][jth feature]
    feature_dictionary = []
    for i in range(class_num):
        feature_dictionary.append({})

    for on_topic in range(class_num):

        #  if we are ettracting class 1's features, we view class1's terms as on topic 
        #  and class2~13's terms are off topic
        train_term_on_topic = []

        for j in range(class_count):
            train_term_on_topic.extend(train_terms[on_topic][j])
        train_term_on_topic = list(set(train_term_on_topic))
        train_term_on_topic.sort()


        for word in train_term_on_topic:
            n11 = 0
            n10 = 0
            n01 = 0
            n00 = 0

            for i in range(class_count):
                if word in train_terms[on_topic][i]:
                        n11+=1
                else:
                        n10+=1

            for off_topic in range(class_num):

                for i in range(class_count):
                    if off_topic == on_topic:
                        break

                    if word in train_terms[off_topic][i]:
                            n01+=1
                    else:
                            n00+=1
            N = n11 + n01 + n10 + n00
            # print("N = ", N)
            pt = (n11 + n01) / N
            p1 = n11 / (n11 + n10)
            p2 = n01 / (n01 + n00)

            likelihood = -2 * math.log( ((pt**n11) * ((1-pt)**n10) * (pt**n01) * ((1-pt)**n00)) / ((p1**n11) * ((1-p1)**n10) * (p2**n01) * ((1-p2)**n00) ), 10)

            feature_dictionary[on_topic][word] = likelihood

    # sort each class' feature with likelihood score
    for i in range(class_num):
        feature_dictionary[i] = sorted(feature_dictionary[i].items(), key=lambda kv: kv[1], reverse=True)

    features = []
    total_big = 0
    enough = False

    for j in range(100):
        big = 0
        if enough:
                break
        for i in range(class_num):


            if feature_dictionary[i][j][1] > 9:
                print(feature_dictionary[i][j])
                features.append(feature_dictionary[i][j][0])
                if len(set(features)) == 500:
                    enough = True
                    break
                big += 1
                total_big += 1
        print()
        print("round: ", j+1 , "; number of selected features:", big)
        print()
        print()
    print("Total selected features:",total_big)
    print("Total selected unreapted features",len(set(features)))
    
    return sorted(list(set(features)))

Vocabulary = feature_selection()


# ## Naïve Bayes algorithm (multinomial model) – training phase

V_size = 500


def TrainMultinomialNB(class_num = 13, class_count = 15, N_train = 13*15, Vocabulary = Vocabulary, V_size = 500):
    prior = []
    for i in range(class_num):
        prior.append(class_count / N_train)

    class_text = []
    for i in range(class_num):
        class_text.append([])

    # concatenate text of all docs In class i
    for i in range(class_num):
        for j in range(class_count):
            class_text[i].extend(tokens(train_nums[i][j]))

    # The number of occurrences of tk in D from class c
    train_Tf_class = []
    for i in range(class_num):
        train_Tf_class.append({})

    for term in Vocabulary:
        for i in range(class_num):
            train_Tf_class[i][term] = class_text[i].count(term)

    class_conprob = []
    for i in range(class_num):
        class_conprob.append({})

    for i in range(class_num):
        for term in Vocabulary:
            class_conprob[i][term] = ( (train_Tf_class[i][term]+1) / (sum(train_Tf_class[i].values()) + V_size) )
    return prior, class_conprob

prior, conprob = TrainMultinomialNB()


# ## Naïve Bayes algorithm (multinomial model) – testing phase

def ApplyMultinomialNB(class_num = 13, class_count = 15, N_all = N_all, prior = prior, conprob = conprob):

    # make all training data index into a list
    train_doc_ids = []
    for class_set in train_nums:
        for doc_id in class_set:
            train_doc_ids.append(doc_id)

    score = []
    for i in range(N_all):
        score.append({})

    for i in range(1, N_all+1):
        if str(i) in train_doc_ids:
            pass

        else:    
            extract_token = tokens(i)
            extract_token = set(extract_token)

            for j in range(class_num):
                score[i-1][j] = math.log(prior[j], 10)

                for term in Vocabulary:

                    if term in extract_token:
                            score[i-1][j] += math.log(conprob[j][term], 10)

    result = []
    class_result = []
    for i in range(len(score)):
        if not score[i]:
            pass
        else:
            for j in score[i]:
                #  find class with highest score, and assign it to that document
                if score[i][j] == max(score[i].values()):
                    result.append((i+1, j+1))
                    class_result.append(j+1)
    return result

result = ApplyMultinomialNB()
output = pd.DataFrame(result) 
output.rename(columns={0: "Id", 1: "Value"}, inplace=True)
output.to_csv("hw3_answer.csv",index=False)


