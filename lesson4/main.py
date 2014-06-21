import urllib2
import sys

TARGET = 'http://crypto-class.appspot.com/po?er='
TEXT_TO_DECIPHER='f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4'

RESULT='The Magic Words are Squeamish Ossifrage' (padded with nines 9)
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    def query(self, q):
        target = TARGET + urllib2.quote(q)    # Create query URL
        req = urllib2.Request(target)         # Send HTTP request to server
        try:
            f = urllib2.urlopen(req)          # Wait for response
        except urllib2.HTTPError, e:          
            print "We got: %d" % e.code       # Print response code
            if e.code == 404:
                return True # good padding
            return False # bad padding


#In deciphering last (4th) block I noted that it's necessary to take IV as first block, rather than the third one! With this block the process 
#doesn't succeed after having deciphered the first byte (the last in the block). This probably happens because the found (first) byte fits in a particular combination 
#of values which satisfies a padding different from the expected one (with last byte equal to one). 

def decipherSingleBlock(blockNumber, paddingOracle):
    firstBlock=blocks[0]

    blockToDecipher=blocks[blockNumber]
    previousBlock=blocks[blockNumber-1]
    plainBlock=''
    decipheredBlock=''
    for step in range(1,17):
        print 'STEP NUMBER: ' + str(step)
        trail=''
        for i in range(0, step-1)[::-1]: 
            trail=''.join((chr(ord(decipheredBlock[i])^step),trail))   #On trail and on tryChar padding is applied! 
                                                                       #Trail contains characters already found with padding

        for it in range(0,256):
        
            tryChar=chr(ord(firstBlock[16-step])^it)                   #On tryChar there is loop to test padding (with trail characters)

            unchanged=firstBlock[ : 16-step]

            if it==0:
                print 'Iterazione numero {0}'.format(it)
                for l in range(0, len(unchanged)): print 'Carattere[{0}] del vettore unchanged: {1}'.format(l, ord(unchanged[l]))
                print 'TryChar: {0}'.format(ord(tryChar))
                for s in range(len(trail)): 
                    print 'Carattere[{0}] del vettore trail trail: {1}'.format(s, ord(trail[s]))

            firstBlockToTx=unchanged + tryChar + trail 
            
            cipherTextToTry=''.join((firstBlockToTx,blockToDecipher))
            if (paddingOracle.query(cipherTextToTry.encode('hex'))):  #Padding oracle wants characters in hex
                foundChar=ord(tryChar)^step
                decipheredBlock=''.join((chr(foundChar),decipheredBlock))
                print 'Trovato carattere! Carattere non ancora completamente decrifrato (int): {0}'.format(ord(decipheredBlock[0]))

                plainChar=chr(foundChar^ord(previousBlock[16-step]))
                print 'Carattere del blocco precedente(int): {0}'.format(ord(previousBlock[16-step]))
                print 'Carattere completamente decrifrato(int): {0}'.format(ord(plainChar))
                plainBlock=''.join((plainChar,plainBlock))
                print 'Parziale del blocco: {0}'.format(plainBlock)
                break 

    print 'Blocco totale: {0}'.format(plainBlock)
    return plainBlock 


blocks=[]

if __name__ == "__main__":
    po = PaddingOracle()
    # po.query(sys.argv[1])       # Issue HTTP query with the given argument

    textToDecipher=TEXT_TO_DECIPHER.decode('hex')
    plainText=''
    
    i=0
    singleBlock=textToDecipher[i*16:(i+1)*16]
    while singleBlock:
       blocks.append(singleBlock)
       i=i+1
       singleBlock=textToDecipher[i*16:(i+1)*16]

    numBlocks=len(blocks)
    print 'Numero blocchi: {0}'.format(numBlocks)

    for j in range(1,numBlocks):

       plainBlock=''
       plainBlock=decipherSingleBlock(j,po)

       print 'PlainBlock: {0}'.format(plainBlock)

       plainText=''.join((plainBlock,plainText))

    print 'PlainText: '
    print plainText


