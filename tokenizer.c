/* char *toLower(char* input)
{
    while (*input != '\0')
    {
        *input = tolower(*input);
        input++;
    }
    return input;
}  */
//optimizer
/* char *toLower(char* input)
{
    int len = strlen(input);
    char *temp = malloc(len + 1);
    strncpy(temp, input, len);
    temp[len] = '\0';
    strlwr(temp);
    return temp;
}  */ 
//optimizer2
/* char *toLower(char* input)
{
    int len = strlen(input);
    for(int i=0; i < len ; i++)
    {
        input[i] = tolower(input[i]);
    } 
    input[len] = '\0';
    return input;
} 
 */
//optimizer3
char *toLower(char* input)
{
    char *result = input; // lưu trữ địa chỉ đầu tiên của chuỗi
    while (*input != '\0')
    {
        *input = tolower(*input);
        input++;
    }
    return result; // trả về địa chỉ đầu tiên của chuỗi
}






//thực hiện giải mã nội dung của yêu cầu
// u_char *urlDecode(u_char *str) 
// {
//   int d = 0; /* whether or not the string is decoded */

//   u_char *dStr = malloc(strlen((const char*)str) + 1);
//   u_char eStr[] = "00"; /* for a hex code */

//   strcpy((char*)dStr, (char*)str);

//   while(!d) {
//     d = 1;
//     int i; /* the counter for the string */
//     int len_dStr = strlen((const char*)dStr);

//     for(i=0;i<len_dStr;++i) {

//       if(dStr[i] == '%') {
//         if(dStr[i+1] == 0)
//           return dStr;

//         if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {

//           d = 0;

//           /* combine the next to numbers into one */
//           eStr[0] = dStr[i+1];
//           eStr[1] = dStr[i+2];

//           /* convert it to decimal */
//           long int x = strtol((char*)eStr, NULL, 16);

//           /* remove the hex */
//           memmove(&dStr[i+1], &dStr[i+3], strlen((char*)&dStr[i+3])+1);

//           dStr[i] = x;
//         }
//       }
//     }
//   }

//   return dStr;
// }

//optimizer1

u_char *urlDecode(u_char *str) 
{
  u_char *dStr = malloc(strlen((const char*)str) + 1); // Khởi tạo bộ nhớ đệm mới
  int d = 0; // biến cờ để kiểm tra xem chuỗi đã được giải mã hoàn toàn hay chưa
  int i = 0; // biến đếm cho chuỗi
  int len_dStr = 0; // độ dài của chuỗi dStr
  
  strcpy((char*)dStr, (char*)str); // Sao chép chuỗi vào bộ nhớ đệm mới

  while(!d) {
    d = 1; // Giả sử chuỗi đã được giải mã hoàn toàn
    len_dStr = strlen((const char*)dStr); // Lấy độ dài của chuỗi dStr

    for(i=0; i<len_dStr; ++i) {

      if(dStr[i] == '%') {
        if(dStr[i+1] == 0)
          return dStr; // Nếu gặp ký tự '%' cuối cùng thì trả về ngay

        if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {

          d = 0; // Nếu gặp mã hex hợp lệ thì cần tiếp tục kiểm tra chuỗi

          // Kết hợp hai số sau dấu '%' thành một số duy nhất
          char hexStr[3];
          hexStr[0] = dStr[i+1];
          hexStr[1] = dStr[i+2];
          hexStr[2] = '\0';

          // Chuyển đổi chuỗi hex thành số nguyên
          long int x = strtol(hexStr, NULL, 16);

          // Di chuyển các ký tự sau mã hex đến vị trí hiện tại
          memmove(&dStr[i+1], &dStr[i+3], strlen((char*)&dStr[i+3])+1);

          dStr[i] = x; // Gán giá trị giải mã vào vị trí đầu tiên của mã hex
        }
      }
    }
  }

  return dStr;
}













//optimizer2
/* u_char* urlDecode(u_char* str) {
  int len = strlen((const char*)str);
  u_char* decoded = (unsigned char*)malloc(len + 1); // Allocate memory for decoded string
  decoded = str;

  for (int i = 0, j = 0; i < len; i++, j++) {
    if (str[i] == '%' && isxdigit(str[i + 1]) && isxdigit(str[i + 2])) {
      int code = 0;
      sscanf((const char*)&str[i + 1], "%2x", &code);
      decoded[j] = code;
      i += 2;
    } else {
      decoded[j] = str[i];
    }
  }

  return decoded;
}

 */

/* int isInstring(char* input, int len, char c)
{
    for(int i=0; i< len; i++)
    {
        if(c == input[i])
            return 1;
    }
    return 0;
} 
 */

// optimizer: OK
int isInstring(char* input, int len, char c)
{
    return memchr(input, c, len) != NULL;
}




/* int isInArr(char* input, char arr[2500][100], int len)
{
    for(int i=0 ; i < len; i++){
        if(strncasecmp(input,arr[i],(int)strlen(input))==0)
            return 1;
    }
    return 0;
}  */
/* //optimizer
int isInArr(char* input, char arr[2500][100], int len)
{
    int input_len = (int)strlen(input);
    char* temp = arr[2500][];
    while (*temp != '\0')
    {
        if (strncasecmp(input, *temp, input_len))
        {
            return 1;
        }
        temp ++;
    }
    return 0;
}  */

//optimizer2
int isInArr(char* input, char** arr, int len_arr)
{
    int len_input = (int)strlen(input);
    for(int i=0 ; i < len_arr; i++){
        if(strncasecmp(input,arr[i],len_input)==0)
            return 1;
    }
    return 0;
} 


/*
int isPathTransform(char* input, int len)
{
    char *pattern = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
    int flag = 1;
    int i=0;
    while( flag == 1 && i < len){
        for(int j=0; j < (int)strlen(pattern); j++ ){
            int result = isInstring(pattern,strlen(pattern),input[i]);
            if(result == 0)
                flag = 0;
        }
        i++;
    }
    return flag;
}*/

//optimizer: OK
/* int isPathTransform(char* input, int len)
{
    char *pattern = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
    int pattern_len = (int)strlen(pattern);
    int flag = 1;
    char* p = input;
    while(*p != '\0'){
        if (!isInstring(pattern, pattern_len, *p))
        {
            flag = 0;
            break;
        }
        p++;
    }
    return flag;
} */
//optimizer2
/* int isPathTransform(char* input, int len)
{
    char *pattern = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
    int span_len = strspn(input, pattern);
    return (span_len == len);
} */
//optimizer3
#include <regex.h>
int isPathTransform(char* input)
{
    regex_t regex;
    int reti;
    char *pattern = "^[0-9a-zA-Z\\-_]+$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti) {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return 0;
    // }

    // Execute the regular expression
    reti = regexec(&regex, input, 0, NULL, 0);

    // Free the regular expression
    regfree(&regex);

    return reti == 0;
} 







/*int isUrlEncoded(char *input, int len)
{
    const char *pattern = "0123456789abcdefABCDEF"
    for(int i=0; i< len; i++)
    {
        if(input[i] == '%' && i > (len-2))
        {
            if(isInstring(input[i+1], pattern) && isInstring(input[i+2],pattern))
                return 1;
        }
    }
    return 0;
}*/



/* int isPureString(char* input, int len)
{
    char *pattern = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    char* p = input;
    int len_pattern = strlen(pattern);
    while (*p != '\0')
    {
        if(!isInstring(pattern, len_pattern, *p))
        {
            return 0;
        }     
        p++;
    }
    
    return 1;

} */
// optimizer
/* int isPureString(char* input, int len)
{
    char *pattern = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    int valid_len = strspn(input, pattern);
    return (valid_len == len);

} */
//optimizer2
int isPureString(char* input)
{
    regex_t regex;
    int reti;
    char *pattern = "^[a-zA-Z\\-]+$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti) {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return 0;
    // }

    // Execute the regular expression
    reti = regexec(&regex, input, 0, NULL, 0);

    // Free the regular expression
    regfree(&regex);

    return reti == 0;
}





// int isHexString(char* input, int len)
// {
//     char *pattern = "0123456789abcdefABCDEF";
//     /*if((int)strlen(input)%2!=0)
//         return 0;
//     int flag = 1;
//     char *pattern = "0123456789abcdefABCDEF";
//     int i=0;
//     while( flag == 1 && i < len){
//         for(int j=0; j < (int)strlen(pattern); j++ ){
//             int result = isInstring(pattern,(int)strlen(pattern),input[i]);
//             if(result==0)
//                 flag = 0;
//         }
//         i++;
//     }
//     return flag;*/
//     for(int i=0; i < len; i++)
//     {
//         if(!isInstring(pattern, strlen(pattern), input[i]))
//            return 0;
//     }
//     return 1;
// }
//optimizer
/* int isHexString(char* input, int len)
{
    char *pattern = "0123456789abcdefABCDEF";
    char* p = input;
    int len_pattern = strlen(pattern);
    while (*p != '\0')
    {
        if(!isInstring(pattern, len_pattern, *p))
        {
            return 0;
        }
        p++;
    }
    return 1;
} */
//optimizer2
/* int isHexString(char* input, int len)
{
    char *pattern = "0123456789abcdefABCDEF";
    int valid_len = strspn(input, pattern);
    return (len == valid_len);
} */
//optimizer3
int isHexString(char* input)
{
    regex_t regex;
    int reti;
    char *pattern = "^[0-9a-fA-F]+$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti) {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return 0;
    // }

    // Execute the regular expression
    reti = regexec(&regex, input, 0, NULL, 0);

    // Free the regular expression
    regfree(&regex);

    return reti == 0;
}


/*
//hash
#define TABLE_SIZE2 22

int isHexString(char* input, int len)
{
    char pattern[TABLE_SIZE2] = "0123456789abcdefABCDEF";
    int hash[TABLE_SIZE2] = {0};

    // Tạo bảng băm từ chuỗi pattern
    for (int i = 0; pattern[i]; i++)
    {
        int index = pattern[i] - '0';
        if (pattern[i] >= 'A' && pattern[i] <= 'F')
            index = pattern[i] - 'A' + 10;
        else if (pattern[i] >= 'a' && pattern[i] <= 'f')
            index = pattern[i] - 'a' + 10;
        hash[index] = 1;
    }

    // Kiểm tra xem các ký tự trong input có nằm trong bảng băm không
    for (int i = 0; i < len; i++)
    {
        int index = input[i] - '0';
        if (input[i] >= 'A' && input[i] <= 'F')
            index = input[i] - 'A' + 10;
        else if (input[i] >= 'a' && input[i] <= 'f')
            index = input[i] - 'a' + 10;
        if (index < 0 || index >= TABLE_SIZE2 || hash[index] == 0)
            return 0;
    }

    return 1;
}
*/


/* 
int isUniString(char* input, int len)
{
    for(int i=0; i < len; i++)
    {
        if((int)input[i] < 32)
            return 1;
    }
    return 0;
} */

//optimizer
/* int isUniString(char* input, int len)
{
    char* p = input;
    while (*p != '\0')
    {
        if((int)(*p) > 32)
        {
            return 0;
        }
        p++;
    }
    
    return 1;
} */
//optimizer2
int isUniString(char* input)
{
    regex_t regex;
    int reti;
    char *pattern = "[\x01-\x1F]";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti) {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return 0;
    // }

    // Execute the regular expression
    reti = regexec(&regex, input, 0, NULL, 0);

    // Free the regular expression
    regfree(&regex);

    return reti == 0;
}







/* int isSenString(char* input, char** KEYWORDS_IN_TRAINING){
    //82: number of keywords
    fprintf(stderr, "\nCall isSenString------------------------------");
    for(int i=0; i < 82; i++)
    {
        if(strlen(KEYWORDS_IN_TRAINING[i]) != strlen(input))
        //strlen(KEYWORDS_IN_TRAINING[i] --> length of keyword
        //strlen(input) --> length of token
        {
            fprintf(stderr, "NOT SENSTRING");
            //return 0;
        }
            

        if(strncasecmp(input,KEYWORDS_IN_TRAINING[i],(int)strlen(input))==0)
        {
            fprintf(stderr, "IS SENSTRING");
            return 1;
        }
    }
    return 0;
} */
//optimizer
/* int isSenString(char* input, char KEYWORDS_IN_TRAINING[100][20]){
    int len_input = (int)strlen(input);
    //82: number of keywords
    for(int i=0; i < 82; i++)
    {
        if(strlen(KEYWORDS_IN_TRAINING[i]) != strlen(input))
        //strlen(KEYWORDS_IN_TRAINING[i] --> length of keyword
        //strlen(input) --> length of token
            return 0;
        if(strncasecmp(input,KEYWORDS_IN_TRAINING[i], len_input)==0)
        {
            return 1;
        }
    }
    return 0;
} */
//optimizer2
// dua bien do dai thay cho 82
/* int isSenString(const char* input, const char** KEYWORDS_IN_TRAINING) {
    int num_keywords = sizeof(KEYWORDS_IN_TRAINING) / sizeof(KEYWORDS_IN_TRAINING[0]);
    int len_input = strlen(input);
    for (int i = 0; i < num_keywords; i++) {
        int len_keyword = strlen(KEYWORDS_IN_TRAINING[i]);
        if (len_keyword != len_input) {
            continue;
        
        if (strncasecmp(input, KEYWORDS_IN_TRAINING[i], len_input) == 0) {
            return 1;
        }
  }
  return 0;
} */
//optimizer3
/* int isSenString(const char* input, char** KEYWORDS_IN_TRAINING) {
  int len_input = strlen(input);
  int num_keywords = (int)((int)sizeof(KEYWORDS_IN_TRAINING) / (int)sizeof(KEYWORDS_IN_TRAINING[0]));
  for (int i = 0; i < num_keywords; i++) {
    int len_keyword = strlen(KEYWORDS_IN_TRAINING[i]);
    if (len_keyword != len_input) {
      continue;
    }
    if (strncasecmp(input, KEYWORDS_IN_TRAINING[i], len_keyword) == 0) {
      return 1;
    }
  }
  return 0;
} */
//optimizer4
int isSenString(const char* input, char** KEYWORDS_IN_TRAINING, int num_keywords) {
  //fprintf(stderr, "\nCall isSenString------------------------------");
  //fprintf(stderr, "\nNumbers of keywords: %i", num_keywords);
  int len_input = strlen(input);
  for (int i = 0; i < num_keywords; i++) 
  {
    int len_keyword = strlen(KEYWORDS_IN_TRAINING[i]);
    if ((len_keyword == len_input) && strncasecmp(input, KEYWORDS_IN_TRAINING[i], len_keyword) == 0) 
      return 1;
  }
  return 0;
}

//optimizer 5
/* int isSenString(char* input, char** KEYWORDS_IN_TRAINING) {
    int i;
    for (i = 0; KEYWORDS_IN_TRAINING[i] != NULL; i++) {
        if (strstr(input, KEYWORDS_IN_TRAINING[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}
 */


/* 
// binary search
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isSenString(char* input, char KEYWORDS_IN_TRAINING[100][20]) {
    int len_input = strlen(input);
    int start = 0;
    int end = 99;

    while (start <= end) {
        int mid = (start + end) / 2;
        int cmp = strncasecmp(input, KEYWORDS_IN_TRAINING[mid], len_input);

        if (cmp == 0) {
            return 1;
        } else if (cmp < 0) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }

    return 0;
}
 */




/* int isNumber(char* input, int len)
{
    char *pattern = "0123456789";
    for(int i=0; i < len; i++)
    {
        if(!isInstring(pattern,10,input[i]))
            return 0;
    }
    return 1;
} */
/* //optimizer
int isNumber(char* input, int len)
{
    char *pattern = "0123456789";
    char* p = input;
    while (*p != '\0')
    {
        if(!isInstring(pattern,10,*p))
        {
            return 0;
        }
        p++;
    }
    return 1;
} */
//optimizer2
/* int isNumber(char* input, int len)
{
    char *pattern = "0123456789";
    int valid_len = strspn(pattern, input);
    return valid_len == len;
} */
//optimizer3
int isNumber(char* input)
{
    regex_t regex;
    int reti;
    char *pattern = "^[0-9]+$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti) {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return 0;
    // }

    // Execute the regular expression
    reti = regexec(&regex, input, 0, NULL, 0);

    // Free the regular expression
    regfree(&regex);

    return reti == 0;
}




/* 
int len_of_string(char* input)
{
    int i;
    for(i=0; input[i] != '\0'; ++i);
    return i;
}
 */


/* int count_slash(char* path, int len)
{
    int count=0;
    for(int i= 0;i < len; i++)
    {
        if(path[i] == '/')
            count++;
    }
    return count;
} */
//optimizer
int count_slash(char* path, int len)
{
    int count=0;
    while (*path != '\0')
    {
        if(*path == '/')
        {
            count++;
        }
        path++;
    }
    return count;
}
//optimizer2
/* int count_slash(char* path, int len)
{
    int count = 0;
    char* p = path;
    while ((p = strchr(p, '/')) != NULL && (p - path) < len)
    {
        count++;
        p++;
    }
    return count;
} */
//optimizer3
/* int count_slash(char* path, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        count += (path[i] & 47) == 47;
    }
    return count;
} */


/* //ta sử dụng một con trỏ để duyệt qua từng ký tự của chuỗi đầu vào
int count_slash(char* path)
{
    int count = 0;
    while (*path != '\0') {
        if (*path == '/') {
            count++;
        }
        path++;
    }
    return count;
}
 */

/* void handle_path(char *parsed_path, char* path, int len, char* punctuations)
{
    parsed_path[0] = '/';
    parsed_path[1] = ' ';
    int k = 2;
    for(int i=1; i< len; i++)
    {
        if(isInstring(punctuations, 29, path[i]))
        {
            parsed_path[k] = ' ';
            parsed_path[k+1] = path[i];
            parsed_path[k+2] = ' ';
            k = k+3;
        }
        else{
            parsed_path[k] = path[i];
            k++;
        }
    }
    parsed_path[k] = ' ';
    parsed_path[k+1] = '?';
    parsed_path[k+2] = '\0';
} */
// optimizer
void handle_path(char *parsed_path, char* path, int len, char* punctuations)
{
    parsed_path[0] = '/';
    parsed_path[1] = ' ';
    int k = 2;
    int punct_flags[256] = {0}; // tạo mảng flags cho bộ lọc ký tự
    for(size_t i=0; i<strlen(punctuations); i++)
    {
        punct_flags[(int)punctuations[i]] = 1;
    }
    for(int i=1; i< len; i++)
    {
        if(punct_flags[(int)path[i]]) // sử dụng toán tử bit để kiểm tra ký tự
        {
            parsed_path[k] = ' ';
            parsed_path[k+1] = path[i];
            parsed_path[k+2] = ' ';
            k = k+3;
        }
        else{
            parsed_path[k] = path[i];
            k++;
        }
    }
    parsed_path[k] = ' ';
    parsed_path[k+1] = '?';
    parsed_path[k+2] = '\0';
}


/*
// hash
void handle_path(char *parsed_path, char* path, int len, char* punctuations)
{
    parsed_path[0] = '/';
    parsed_path[1] = ' ';

    // Tạo mảng băm để kiểm tra ký tự nằm trong chuỗi "punctuations"
    int punct_map[256] = {0};
    for (int i = 0; i < strlen(punctuations); i++) {
        punct_map[punctuations[i]] = 1;
    }

    int k = 2;
    for (int i = 1; i < len; i++) {
        if (punct_map[path[i]]) {
            parsed_path[k++] = ' ';
            parsed_path[k++] = path[i];
            parsed_path[k++] = ' ';
        } else {
            memcpy(&parsed_path[k], &path[i], 1);
            k++;
        }
    }

    parsed_path[k] = ' ';
    parsed_path[k+1] = '?';
    parsed_path[k+2] = '\0';
}
*/


/* void handle_data(char* parsed_data, char* data, int len, char* punctuations)
{
    //char *parsed_data = malloc(1000);
    //parsed_data[0] = '?';
    parsed_data[0] = ' ';
    int k = 1;
    for(int i = 0; i< len; i++)
    {
        if(isInstring(punctuations, 29, data[i]))
        {
            parsed_data[k] = ' ';
            parsed_data[k+1] = data[i];
            parsed_data[k+2] = ' ';
            k = k+3;
        }
        else{
            parsed_data[k] = data[i];
            k++;
        }
    }
    parsed_data[k] = '\0';
} */
//optimizer
void handle_data(char* parsed_data, char* data, int len, char* punctuations)
{
    parsed_data[0] = ' ';
    int k = 1;
    int punct_flags[256] = {0}; // tạo mảng flags cho bộ lọc ký tự
    for(size_t i=0; i<strlen(punctuations); i++)
    {
        punct_flags[(int)punctuations[i]] = 1;
    }
    for(int i = 0; i< len; i++)
    {
        if(punct_flags[(int)data[i]]) // sử dụng toán tử bit để kiểm tra ký tự
        {
            parsed_data[k] = ' ';
            parsed_data[k+1] = data[i];
            parsed_data[k+2] = ' ';
            k = k+3;
        }
        else{
            parsed_data[k] = data[i];
            k++;
        }
    }
    parsed_data[k] = '\0';
}


/*
//hash
void handle_data(char* parsed_data, char* data, int len, char* punctuations)
{
    parsed_data[0] = ' ';

    // Tạo mảng băm để kiểm tra ký tự nằm trong chuỗi "punctuations"
    int punct_map[256] = {0};
    for (int i = 0; i < strlen(punctuations); i++) {
        punct_map[punctuations[i]] = 1;
    }

    int k = 1;
    for (int i = 0; i < len; i++) {
        if (punct_map[data[i]]) {
            parsed_data[k++] = ' ';
            parsed_data[k++] = data[i];
            parsed_data[k++] = ' ';
        } else {
            memcpy(&parsed_data[k], &data[i], 1);
            k++;
        }
    }

    parsed_data[k] = '\0';
}
*/


/* int find_position_of_str(char pattern[320][100], char* str, int len_pattern)
{

    int len_str = (int)strlen(str);
    for(int i=0; i< len_pattern; i++)
    {
        if(strncasecmp(pattern[i],str,len_str)==0)
        {
            return i +1;
            
         }
    }
    return -1;
} */
/* //optimizer
int find_position_of_str_helper(char pattern[320][100], char* str, int start, int end) {
    if (start > end) {
        return -1;
    }
    if (strncasecmp(pattern[start], str, strlen(str)) == 0) {
        return start + 1;
    }
    return find_position_of_str_helper(pattern, str, start + 1, end);
}

int find_position_of_str(char pattern[320][100], char* str, int len) {
    return find_position_of_str_helper(pattern, str, 0, len - 1);
}
 */
//optimizer2
/* int cmpfunc(const void* a, const void* b)
{
    const char* aa = *(const char**)a;
    const char* bb = *(const char**)b;
    return strcasecmp(aa, bb);
} */
// optimizer3
/* int find_position_of_str(char** pattern, char* str, int len_pattern)
{
    int len_str = (int)strlen(str);
    for(int i = 0; i < len_pattern; i++)
    {
        if(strncasecmp(pattern[i], str, len_str) == 0)
        {
            return i + 1;
        }
    }
    return -1;
}
 */
//optimizer4
int find_position_of_str(char** pattern, char* str, int num_patterns)
{
    if (pattern == NULL || str == NULL)
    {
        // Error: pattern or str is NULL
        return -1;
    }

    for (int i = 0; i < num_patterns; i++)
    {
        if (strncasecmp(pattern[i], str, num_patterns) == 0)
        {
            return i + 1;
        }
    }

    return -1;
}

/* int find_position_of_str_v2(char** pattern, char c, int num_patterns)
{
    if (pattern == NULL)
    {
        // Error: pattern is NULL
        return -1;
    }

    for (int i = 0; i < num_patterns; i++)
    {
        if (strchr(pattern[i], c) != NULL)
        {
            return i + 1;
        }
    }

    return -1;
}
 */





/* void tokenizer_path1(int *token_arr, char* parsed_path, char** pattern, int num_patterns, char *punctuations, char** ext, int len_ext, int *len_path_arr)
{
    //int *token_arr = malloc(100);
    int i = 0;
    int position_pathstring = find_position_of_str(pattern, "pathstring", num_patterns);
    //int position_slash = find_position_of_str(pattern, "/", len);
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    char* token = strtok(parsed_path," ");
    char* pre_string = NULL;
    while( token != NULL ) {
        if((int)strlen(token)==1 && isInstring(punctuations, 29, token[0]))
        {
                token_arr[i] = find_position_of_str(pattern, token , num_patterns);
        }
        else if(isInArr(token,ext,len_ext) && pre_string != NULL && (pre_string[0]=='.'))
        {
            int position_ext = find_position_of_str(pattern, token, num_patterns);
            if(position_ext==-1)
                position_ext = find_position_of_str(pattern, token, num_patterns);
            if(position_ext==-1)
                token_arr[i] = position_purestring;
            else
                token_arr[i] = position_ext;
        }
        else
        {
            token_arr[i] = position_pathstring;
        }
        i++;
        if(pre_string != NULL)
            free(pre_string);
        pre_string = malloc((int)strlen(token)+1);
        pre_string = memcpy(pre_string,token,(int)strlen(token));
        token = strtok(NULL, " ");
        *len_path_arr = i;
    }

}
 */


/* void tokenizer_path(int *token_arr, char* parsed_path, char** pattern, int num_patterns, char *punctuations, char** ext, int len_ext, int *len_path_arr)
{
    //int *token_arr = malloc(100);
    int i = 0;
    int position_pathstring = find_position_of_str(pattern, "pathstring", num_patterns);
    //int position_slash = find_position_of_str(pattern, "/", len);
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    char* token = strtok(parsed_path," ");
    char* pre_string = NULL;
    while( token != NULL ) {
        if((int)strlen(token)==1 && isInstring(punctuations, 29, token[0]))
        {
                token_arr[i] = find_position_of_str(pattern, token , num_patterns);
        }
        else if(isInArr(token,ext,len_ext) && pre_string != NULL && (pre_string[0]=='.'))
        {
            int position_ext = find_position_of_str(pattern, token, num_patterns);
            if(position_ext==-1)
                position_ext = find_position_of_str(pattern, token, num_patterns);
            if(position_ext==-1)
                token_arr[i] = position_purestring;
            else
                token_arr[i] = position_ext;
        }
        else
        {
            token_arr[i] = position_pathstring;
        }
        i++;
        if(pre_string != NULL)
            free(pre_string);
        pre_string = malloc((int)strlen(token)+1);
        pre_string = memcpy(pre_string,token,(int)strlen(token));
        token = strtok(NULL, " ");
        *len_path_arr = i;
    }

} */
// optimizer
void tokenizer_path(int *token_arr, char* parsed_path, char** pattern, int num_patterns, char *punctuations, char** ext, int len_ext, int *len_path_arr)
{
    int i = 0;
    int position_pathstring = find_position_of_str(pattern, "pathstring", num_patterns);
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    char* token = strtok(parsed_path," ");
    char* pre_string = NULL;
    int pre_string_len = 0;
    while( token != NULL ) {
        if((int)strlen(token)==1 && isInstring(punctuations, 29, token[0]))
        {
            token_arr[i] = find_position_of_str(pattern, token , num_patterns);
        }
        else if(isInArr(token,ext,len_ext) && pre_string != NULL && (pre_string[0]=='.'))
        {
            int position_ext = find_position_of_str(pattern, token, num_patterns);
            if(position_ext==-1)
                token_arr[i] = position_purestring;
            else
                token_arr[i] = position_ext;
        }
        else
        {
            token_arr[i] = position_pathstring;
        }
        i++;
        pre_string_len = strlen(token);
        if(pre_string != NULL)
            pre_string = realloc(pre_string, pre_string_len+1);
        else
            pre_string = malloc(pre_string_len+1);
        memcpy(pre_string,token,pre_string_len+1);
        token = strtok(NULL, " ");
    }
    *len_path_arr = i;
    free(pre_string);
}


/* /// Original function ///
void tokenizer_data(int *token_arr, char* parsed_data, char** pattern, int num_patterns, int *len_path_arr, char *punctuations, char** KEYWORDS_IN_TRAINING, int num_keywords)
{
    //int *token_arr = malloc(100);
    //char serve_token_arr[100][100];
    int i = 0;
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    int position_unistring = find_position_of_str(pattern, "unistring", num_patterns);
    int position_hexstring = find_position_of_str(pattern, "hexstring", num_patterns);
    //int position_senstring = find_position_of_str(pattern, "senstring", len);
    int position_number = find_position_of_str(pattern, "numbers", num_patterns);
    int position_mixstring = find_position_of_str(pattern, "mixstring", num_patterns);

    char* token = strtok(parsed_data," ");
    while(token != NULL ) {
        if((int)strlen(token)==1 && isInstring(punctuations,29, token[0]))
        {
            fprintf(stderr,"\npunc %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else if(isSenString(token,KEYWORDS_IN_TRAINING, num_keywords))
        {
            fprintf(stderr,"\nsen %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else if(isNumber(token))
        {
            fprintf(stderr,"\nnumber %s",token);
            token_arr[i] = position_number;
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else if(isHexString(token))
        {
            fprintf(stderr,"\nhex %s",token);
            token_arr[i] = position_hexstring;
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else if(isPureString(token))
        {
            fprintf(stderr,"\npure %s",token);
            token_arr[i] = position_purestring;
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else if(isUniString(token))
        {
            fprintf(stderr,"\nuni %s",token);
            token_arr[i] = position_unistring;
            i++;
            token = strtok(NULL, " ");
            continue;
        }
        else
        {
            fprintf(stderr,"\nmix %s",token);   
            token_arr[i] = position_mixstring;
            i++;
            token = strtok(NULL, " ");
            continue;
        }     
    }
    *len_path_arr = i;
} */
// optimizer
void tokenizer_data(int *token_arr, char* parsed_data, char** pattern, int num_patterns, int *len_path_arr, char *punctuations, char** KEYWORDS_IN_TRAINING, int num_keywords)
{
    int i = 0;
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    int position_unistring = find_position_of_str(pattern, "unistring", num_patterns);
    int position_hexstring = find_position_of_str(pattern, "hexstring", num_patterns);
    int position_number = find_position_of_str(pattern, "numbers", num_patterns);
    int position_mixstring = find_position_of_str(pattern, "mixstring", num_patterns);

    char* token = strtok(parsed_data," ");
    int token_length = strlen(token);
    while(token != NULL ) {
        if(token_length == 1 && isInstring(punctuations,29, token[0]))
        {
            fprintf(stderr,"\npunc %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
        }
        else if(isSenString(token,KEYWORDS_IN_TRAINING, num_keywords))
        {
            fprintf(stderr,"\nsen %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
        }
        else if(isNumber(token))
        {
            fprintf(stderr,"\nnumber %s",token);
            token_arr[i] = position_number;
        }
        else if(isHexString(token))
        {
            fprintf(stderr,"\nhex %s",token);
            token_arr[i] = position_hexstring;
        }
        else if(isPureString(token))
        {
            fprintf(stderr,"\npure %s",token);
            token_arr[i] = position_purestring;
        }
        else if(isUniString(token))
        {
            fprintf(stderr,"\nuni %s",token);
            token_arr[i] = position_unistring;
        }
        else
        {
            fprintf(stderr,"\nmix %s",token);   
            token_arr[i] = position_mixstring;
        }
        i++;
        token = strtok(NULL, " ");
        if (token != NULL) {
            token_length = strlen(token);
        }
    }
    *len_path_arr = i;
}


/* void tokenizer_data1(int *token_arr, char* parsed_data, char** pattern, int num_patterns, int *len_path_arr, char *punctuations, char** KEYWORDS_IN_TRAINING, int num_keywords)
{
    //int *token_arr = malloc(100);
    //char serve_token_arr[100][100];
    int i = 0;
    int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
    int position_unistring = find_position_of_str(pattern, "unistring", num_patterns);
    int position_hexstring = find_position_of_str(pattern, "hexstring", num_patterns);
    //int position_senstring = find_position_of_str(pattern, "senstring", len);
    int position_number = find_position_of_str(pattern, "numbers", num_patterns);
    int position_mixstring = find_position_of_str(pattern, "mixstring", num_patterns);

    char* rest = parsed_data; // con trỏ để theo dõi vị trí trong chuỗi
    char* token;
    //char* blank = " ";
    while((token = strsep(&rest, punctuations)) != NULL) {
        if((int)strlen(token)==1 && isInstring(punctuations,29, *token))
        {
            fprintf(stderr,"\npunc %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
            i++;
            continue;
        }
        else if(isSenString(token,KEYWORDS_IN_TRAINING, num_keywords))
        {
            fprintf(stderr,"\nsen %s",token);
            token_arr[i] = find_position_of_str(pattern, token, num_patterns);
            i++;

            continue;
        }
        else if(isNumber(token))
        {
            fprintf(stderr,"\nnumber %s",token);
            token_arr[i] = position_number;
            i++;

            continue;
        }
        else if(isHexString(token))
        {
            fprintf(stderr,"\nhex %s",token);
            token_arr[i] = position_hexstring;
            i++;

            continue;
        }
        else if(isPureString(token))
        {
            fprintf(stderr,"\npure %s",token);
            token_arr[i] = position_purestring;
            i++;

            continue;
        }
        else if(isUniString(token))
        {
            fprintf(stderr,"\nuni %s",token);
            token_arr[i] = position_unistring;
            i++;
            continue;
        }
        else 
        {
            fprintf(stderr,"\nmix %s",token);   
            token_arr[i] = position_mixstring;
            i++;    
            continue;
        }
    }
    *len_path_arr = i;
} */


// void tokenizer_data1(int *token_arr, char* parsed_data, char** pattern, int num_patterns, int *len_path_arr, char *punctuations, char** KEYWORDS_IN_TRAINING)
// {
//     //int *token_arr = malloc(100);
//     //char serve_token_arr[100][100];
//     int i = 0;
//     int position_purestring = find_position_of_str(pattern, "purestring", num_patterns);
//     int position_unistring = find_position_of_str(pattern, "unistring", num_patterns);
//     int position_hexstring = find_position_of_str(pattern, "hexstring", num_patterns);
//     //int position_senstring = find_position_of_str(pattern, "senstring", len);
//     int position_number = find_position_of_str(pattern, "numbers", num_patterns);
//     int position_mixstring = find_position_of_str(pattern, "mixstring", num_patterns);

//     //Chia du lieu da xu ly thanh cac chuoi nho dua vao ky tu khoang trang " "
//     char* token = strtok(parsed_data," ");

//     /* char* token_punctuations;
//     char* delimiters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // Các ký tự không đặc biệt
//     //char* param = parsed_data;
//     token_punctuations = strtok(parsed_data, delimiters);

//     while (token_punctuations != NULL) {
//         fprintf(stderr,"\nToken punctuation: %s", token_punctuations);
//         token_punctuations = strtok(NULL, delimiters);
//     } */
        

    


//     /*int index_punctuations = 0;
//     for (int j =0; j < (int)strlen(parsed_data); j++) {
//         if (isInstring(punctuations,29, parsed_data[j])) {
//             token_punctuations[index_punctuations] = (char)parsed_data[j]; 
//             fprintf(stderr,"\nToken punctuation: %s", token_punctuations[index_punctuations]);      
//         }
//         index_punctuations++;
//     } */


//  /*    // Chia du lieu da xu ly thanh cac chuoi nho dua vao ky tu puntuations
//     char* token = strtok(parsed_data, punctuations);
//     fprintf(stderr,"\nToken: %s", token); */
    



    

//     // Vong lap while duyet qua tung chuoi nho xem no thuoc vao truong hop nao
//     while(token != NULL ) {
//         // Neu la ky tu dac biet
//         if((int)strlen(token)==1 && isInstring(punctuations,29, token[0]))
//         {
//             //fprintf(stderr,"\nfound=%s",token);
//             //token_arr[i] = find_position_of_str(pattern, token, num_patterns);
//             //printf("\nfound=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");
//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else if(isSenString(token,KEYWORDS_IN_TRAINING))
//         {
//             //fprintf(stderr,"\nsen=%s",token);
//             token_arr[i] = find_position_of_str(pattern, token, num_patterns);
//             //printf("\nsen=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");

//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else if(isNumber(token,(int)strlen(token)))
//         {
//             //fprintf(stderr,"\nnumber=%s",token);
//             token_arr[i] = position_number;
//             //printf("\nnumber=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");

//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else if(isHexString(token,(int)strlen(token)))
//         {
//             //fprintf(stderr,"\nhex=%s",token);
//             token_arr[i] = position_hexstring;
//             //printf("\nhex=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");

//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else if(isPureString(token,(int)strlen(token)))
//         {
//             //fprintf(stderr,"pure=%s",token);
//             token_arr[i] = position_purestring;
//             //printf("\npure=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");
//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else if(isUniString(token,(int)strlen(token)))
//         {
//             //fprintf(stderr,"\nuni=%s",token);
//             token_arr[i] = position_unistring;
//             //printf("\nuni=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");

//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
//         else
//         {
//             //fprintf(stderr,"\nmix=%s",token);
//             token_arr[i] = position_mixstring;
//             //printf("\nmix=%i",token_arr[i]);
//             i++;
//             //token = strtok(NULL, punctuations);
//             token = strtok(NULL, " ");

//             fprintf(stderr,"\nToken: %s", token);
//     	    *len_path_arr = i;
//             continue;
//         }
        
//     }
//     //return token_arr;
// }
