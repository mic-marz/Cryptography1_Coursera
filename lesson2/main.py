#! /usr/bin/python

import os
from Crypto.Cipher import AES
from Crypto.Util import Counter


msg1='4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81'
key1='140b41b22a29beb4061bda66b6747e14'
mode1=AES.MODE_CBC

msg2='5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253'
key2='140b41b22a29beb4061bda66b6747e14'
mode2=AES.MODE_CBC

msg3='69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329'
key3='36f18357be4dbd77f050515c73fcf9f2'
mode3=AES.MODE_CTR

msg4='770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451'
key4='36f18357be4dbd77f050515c73fcf9f2'
mode4=AES.MODE_CTR



def decipher(ciphermsg, keytext, mode):
    hexIv=ciphermsg[:32]
    iv=hexIv.decode('hex')
    realCiphermsg=ciphermsg[32:].decode('hex')
    key=keytext.decode('hex')

    if mode==AES.MODE_CBC:
        aesObj=AES.new(key, mode, iv)
    elif mode==AES.MODE_CTR:
        first_value=int(hexIv, base=16)
        ctrObj=Counter.new(128,initial_value=first_value)
        aesObj=AES.new(key, mode, counter=ctrObj)
    else:
        print 'Invalid encryption mode specified!'
        os.exit(1)

    plainmsg=aesObj.decrypt(realCiphermsg)

    if mode==AES.MODE_CBC:                        # CBC padding handling
        lastValue=ord(plainmsg[-1])
        if lastValue >=1 or lastValue <=16:       #PKCS5 padding scheme
            paddingLength=lastValue
            plainmsg=plainmsg[:-paddingLength]

    print 'Msg deciphered: ' + plainmsg
    





if __name__=='__main__':
    msgs=(msg1, msg2, msg3, msg4)
    keys=(key1, key2, key3, key4)   
    modes=(mode1, mode2, mode3, mode4)

    for (msg, key, mode) in zip(msgs, keys, modes):
        decipher (msg, key, mode)


