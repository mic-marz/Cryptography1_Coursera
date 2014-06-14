#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The method used here is based on successive tunings and hypotesis.
 * First hypotesis: Initially assume that plaintext is made up of alphabetic characters. In successive tunings it's possibile
 *                  to enlarge character set with symbols and numbers. Supposing only alphabetic characters (more probable than
 *                  symbols in common text) is useful for the following hypotesis.
 * Second hypotesis: Even if in successive algorithm iterations character set is enlarged with symbols, it's assumed that ' ' (space
 * 		     character) is obtained by xoring an alphabetic character with itself with lowercase/uppercase difference. 
 * Third hypotesis: Symbols set is ordered by means of occurence probability in common text.
 *
 * For these approximative hypotesis could happen that some key bytes won't be found and others one will be supposed wronlgy.  
 */ 		    

#define NUMBER_OF_MSGS 11
#define NUMBER_OF_SYMBOLS 29    /* +1 for space character */

struct match {
       char exactMatches[NUMBER_OF_MSGS];    /* complete match with no lowercase/uppercase difference */
       char exactMatchesTotal;
       char noCaseMatches[NUMBER_OF_MSGS];   /* partial match with lowercase/uppercase difference */
       char noCaseMatchesTotal;
       char matchesTotal;
       char msgIndex;
       };

static struct match matchInfo[NUMBER_OF_MSGS];  /* This array will be reordered! */


#define MSG1 "315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b16349c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc41556bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d524400a19b159610b11ef3e\0"

#define MSG2 "234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b028aa76eb7b4ab24171ab3cdadb8356f\0"

#define MSG3 "32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b07df44ba6e9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257527dc29398f5f3251a0d47e503c66e935de81230b59b7afb5f41afa8d661cb\0"

#define MSG4 "32510ba9aab2a8a4fd06414fb517b5605cc0aa0dc91a8908c2064ba8ad5ea06a029056f47a8ad3306ef5021eafe1ac01a81197847a5c68a1b78769a37bc8f4575432c198ccb4ef63590256e305cd3a9544ee4160ead45aef520489e7da7d835402bca670bda8eb775200b8dabbba246b130f040d8ec6447e2c767f3d30ed81ea2e4c1404e1315a1010e7229be6636aaa\0"

#define MSG5 "3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe56808c213f17c81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d562e8e9582f5ef375f0a4ae20ed86e935de81230b59b73fb4302cd95d770c65b40aaa065f2a5e33a5a0bb5dcaba43722130f042f8ec85b7c2070\0"

#define MSG6 "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a19d84aba34d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512472e8e8f8db3f9635c1949e640c621854eba0d79eccf52ff111284b4cc61d11902aebc66f2b2e436434eacc0aba938220b084800c2ca4e693522643573b2c4ce35050b0cf774201f0fe52ac9f26d71b6cf61a711cc229f77ace7aa88a2f19983122b11be87a59c355d25f8e4\0"

#define MSG7 "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b01c909ba7696cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea125d298e8883f5e9305f4b44f915cb2bd05af51373fd9b4af511039fa2d96f83414aaaf261bda2e97b170fb5cce2a53e675c154c0d9681596934777e2275b381ce2e40582afe67650b13e72287ff2270abcf73bb028932836fbdecfecee0a3b894473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4ce\0"

#define MSG8 "315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3\0"

#define MSG9 "271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027\0"

#define MSG10 "466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83\0"

#define MSG11 "32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac650e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef121336cb85ccb8f3315f4b52e301d16e9f52f904\0"

static char *inputArray[NUMBER_OF_MSGS] = {MSG1, MSG2, MSG3, MSG4, MSG5, MSG6, MSG7, MSG8, MSG9, MSG10, MSG11}; 

struct msgData {
       int lengthMsg;
       char *msg;
       };

static struct msgData msgArray[NUMBER_OF_MSGS];

#define TARGET_TEXT_LENGTH_IN_HEX 166
#define TARGET_TEXT_LENGTH TARGET_TEXT_LENGTH_IN_HEX/2   /* Length of MSG11 */
static char plainText[TARGET_TEXT_LENGTH]="";
static char keyText[TARGET_TEXT_LENGTH]="";
static char msg11Guessed[]="The secret message is: when using a stream cipher, never use the key more than once";   /* FINAL RESULT!!! */


static char orderedSymbolsSequence[NUMBER_OF_SYMBOLS + 1 ]={' ', 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'c', 'u', 'm', 'w', 'f', 'g', 'y', 'p', 'b', 'v', 'k', 'j', 'x', 'q', 'z', 
'(', ':', 
/*';', '{',
',', '.', '}', ')', '*', '1', '2', 
 '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '"', '#', '$', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', '\'', ']', '^', '_', '`', '{', '|', '}', '~', */
'\0'};
static char flagsForSymbols[NUMBER_OF_SYMBOLS + 1]="";    /* One mark for each letter supposed in single iteration by other groups of msgs (no matter for case) */

struct supposing {
       char currentIndex;        /* Index in orderedSymbolsSequence of letter currently supposed for a group of msgs */ /* It's a useful cross-iteration information */
       };

static struct supposing supposingArray[NUMBER_OF_MSGS]={{-1}};

static char jumpMsgs[NUMBER_OF_MSGS]="";    /* optimized algorithm */


void initializeMatchInfo(void );
void initializeSupposingArray(int );
int compareMatchStruct(const void *first, const void *second);
void fillMsgArray(void );
void convertStrings(char *input, char *output);
void convertChar(char *inputChar, char *outputChar);
void decipherTexts(void );



int main()
{
  int k=0, l=0, m=0, n=0, j=0, s=0, p=0, r=0, t=0;
  char xorChar='\0', supposedPlainChar='\0', endSearchForCharacter='\0', firstCompare='\0', secondCompare='\0', temp='\0';

  fillMsgArray();

  for (m=0; m<TARGET_TEXT_LENGTH; m++)   /* For every character */
  {
    #ifdef DEBUG 
    printf("\n<------- Char number: %d --------------------->\n", m);
    #endif
    endSearchForCharacter=0;
    bzero(flagsForSymbols, sizeof(flagsForSymbols));
    bzero(jumpMsgs, sizeof(jumpMsgs));
    initializeMatchInfo();
    initializeSupposingArray(0);
    
    t=0;
    for(k=0; k<NUMBER_OF_MSGS; k++)
    {
      if (jumpMsgs[k])
        continue;

      matchInfo[t].msgIndex=k;
      matchInfo[t].exactMatches[k]=1;   /* K msg sign itself with an exact match */

      for(l=k+1; l<NUMBER_OF_MSGS; l++)
      {
        if (jumpMsgs[l])
          continue;

        xorChar = (msgArray[k].msg[m]) ^ (msgArray[l].msg[m]);

        if ((!xorChar) || (xorChar==32))
        {
          jumpMsgs[l]=1;

          if (!xorChar)
          {
            matchInfo[t].exactMatches[l]=1;
            matchInfo[t].exactMatchesTotal++;
          }
          else 
          {
            matchInfo[t].noCaseMatches[l]=1;
            matchInfo[t].noCaseMatchesTotal++;
          }
          matchInfo[t].matchesTotal++;

        }
      }
      t++;
    }

     
    // Structures are now filled! 

    qsort(matchInfo, NUMBER_OF_MSGS, sizeof(struct match), compareMatchStruct);  
 

    // First hypotesys on the first group of msgs  
    // Here currentIndex for the first group of messages is 0  
    supposingArray[0].currentIndex++;  
    while((supposingArray[0].currentIndex < NUMBER_OF_SYMBOLS) && (!endSearchForCharacter))
    { 
      flagsForSymbols[supposingArray[0].currentIndex]=1;
      // The key byte supposed doesn't consider the 6th bit (0x20)  
      keyText[m]=(orderedSymbolsSequence[supposingArray[0].currentIndex]) ^ msgArray[matchInfo[0].msgIndex].msg[m]; 
      
      #ifdef DEBUG 
      printf("\nChar index: %d, CurrentIndex for first matchInfo: %d, Ordered alphabet indexed by currentIndex: %c, Initial keyChar supposed from msg=%d: %d\n",
      m, supposingArray[0].currentIndex, orderedSymbolsSequence[supposingArray[0].currentIndex], matchInfo[0].msgIndex, keyText[m]);
      #endif

      initializeSupposingArray(1); 
      firstCompare=0;
      secondCompare=0;
      s=1; 
      while((s >= 1) && (s<NUMBER_OF_MSGS) && (matchInfo[s].msgIndex >=0 ))
      {
        #ifdef DEBUG 
        printf("\nMatchInfo structure index=%d from msg:%d, Matches Total: %d !\n",s, matchInfo[s].msgIndex, matchInfo[s].matchesTotal);
	#endif
        // Obtain the first more probable letter for the number s group of messages  
        supposedPlainChar = keyText[m] ^ msgArray[matchInfo[s].msgIndex].msg[m];

        temp=supposingArray[s].currentIndex;  // It's useful when returning to this s value from a major one (no compare true for any subsequent s value)
        r=++temp;
        do 
        { 
          firstCompare=0;
          secondCompare=0;

          for (p=r; (p<NUMBER_OF_SYMBOLS) && (flagsForSymbols[p]==1); p++)
          {
          }
          r=p+1; 

          if (p<NUMBER_OF_SYMBOLS)
          {
            firstCompare=(supposedPlainChar == orderedSymbolsSequence[p]);
            secondCompare=((supposedPlainChar ^0x20) == orderedSymbolsSequence[p]);
          }
          else 
            break;
        }
        while( !(firstCompare || secondCompare) );
        
        if (r > NUMBER_OF_SYMBOLS)   // No compare true! 
        {
          s--;                        // Backward 
          flagsForSymbols[supposingArray[s].currentIndex]=0;     // Operation valid even for the first group of msgs 

          #ifdef DEBUG 
          printf("\nMatchInfo structure index=%d, No Match Found! Going back to previous s value\n", s+1);
          printf("\nNew s value: %d, flagsForSymbols[supposingArray[0].currentIndex] : %d\n", s, flagsForSymbols[supposingArray[0].currentIndex]);
	  #endif
        }
        else                         // Any compare true 
        {
          supposingArray[s].currentIndex=p;
          flagsForSymbols[supposingArray[s].currentIndex]=1;

          #ifdef DEBUG 
          printf("\nMatchInfo structure index=%d, Match Found! Character selected: %c\n", s, orderedSymbolsSequence[supposingArray[s].currentIndex]);  
	  #endif
          s++;   // Forward 
        }
      }
      
      if (firstCompare || secondCompare || s==1 )    // Compare values are relative to the last value of s for which totalMatches > 0 
                                                     // s==1 is for when msgs groups that follow the first have msgIndex = -1
      { 
        #ifdef DEBUG 
        printf("\nChar index=%d, end search successfully!\n",m);
	#endif
        endSearchForCharacter=1;
      }
      else 
      {
        supposingArray[0].currentIndex++;
      }
    }
   
    if (supposingArray[0].currentIndex == NUMBER_OF_SYMBOLS)
    {
      keyText[m]=0xFF;
      #ifdef DEBUG 
      printf("\nNo byte value found for key[%d] !\n", m);
      #endif
    }
  }
  
  printf("\nShowing deciphered texts!\n");
  decipherTexts();

  printf("\n\nShowing deciphered texts with msg11 guessed!\n");
  for (m=0; m<TARGET_TEXT_LENGTH; m++)
  {
    keyText[m] = msg11Guessed[m] ^ msgArray[10].msg[m];
  }
  decipherTexts();


  return 0; 
}



void decipherTexts(void )
{
  int s=0, m=0;
  for (s=0; s<NUMBER_OF_MSGS; s++)
  {
    printf("\nMSG %d:\n",s);
    for (m=0; m<TARGET_TEXT_LENGTH; m++)
    {
      plainText[m] = keyText[m] ^ msgArray[s].msg[m];
    }
    printf("\n%s\n", plainText);
  }
}


void initializeSupposingArray(int startElement )
{
  int k;
  for(k=startElement; k<NUMBER_OF_MSGS; k++)
  {
    supposingArray[k].currentIndex=-1;
  }
}

void initializeMatchInfo(void )
{
  int k;

  bzero(matchInfo, sizeof(matchInfo));
  for(k=0; k<NUMBER_OF_MSGS; k++)
  {
    matchInfo[k].msgIndex=-1;
  }
}

int compareMatchStruct(const void *first, const void *second) /* Descending order */
{
  if (((struct match *)first)->matchesTotal < ((struct match *)second)->matchesTotal)
    return 1;
  else if (((struct match *)first)->matchesTotal > ((struct match *)second)->matchesTotal)
         return -1;
      
  return 0;
}


void fillMsgArray(void ) 
{
  int i=0;
 
  
  for (i=0; i<NUMBER_OF_MSGS; i++)
  {
    msgArray[i].lengthMsg=strlen(inputArray[i])/2;
    msgArray[i].msg = (char *)malloc(msgArray[i].lengthMsg + 1);
    bzero(msgArray[i].msg, sizeof(msgArray[i].msg));

    convertStrings(inputArray[i], msgArray[i].msg);  
    msgArray[i].msg[msgArray[i].lengthMsg]='\0';
  }
}


void convertStrings(char *input, char *output)
{
  int j, i, length = strlen(input);
  char leftHalf, rigthHalf;

  for(i=0; i<length; i++)
  {
    leftHalf = 0;
    rigthHalf = 0;
    
    #ifdef DEBUG
    printf("\ni: %d, Carattere input: %d\n", i, input[i]);
    #endif  

    convertChar(input+i, &leftHalf);
    #ifdef DEBUG
    printf("\nCarattere left convertito: %d \n", leftHalf);
    #endif  

    leftHalf <<= 4;

    #ifdef DEBUG
    printf("\nCarattere left convertito e traslato: %d \n", leftHalf);
    #endif  

    j=i/2;
    i++;

    #ifdef DEBUG
    printf("\ni: %d, Carattere input: %d\n", i, input[i]);
    #endif  

    convertChar(input+i, &rigthHalf);

    #ifdef DEBUG
    printf("\nCarattere rigth convertito: %d \n", rigthHalf);
    #endif  

    output[j] = leftHalf | rigthHalf;

    #ifdef DEBUG
    printf("\nCarattere output convertito: %d \n", output[j]);
    #endif  
  }
}

int testConvertStrings()
{
 char stringInput[3]="";
 char stringOutput[3]="";

 strncpy(stringInput, inputArray[0] + 2 , 2);
 convertStrings(stringInput, stringOutput);
 printf("\nValore di stringOutput: %d\n", stringOutput[0]);

 return 0;

}


void convertChar(char *inputChar, char *outputChar)
{
   char aux[2]="\0\0"; 
   aux[0] = *(inputChar);
    
   *outputChar = (char)strtol(aux, NULL, 16);
}
