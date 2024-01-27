import subprocess

def split_string(s, n):
    return [s[i:i+n] for i in range(0, len(s), n)]

# 遍历明文 16到32
for i in range(16, 32):
    # 将十进制明文转换为16位16进制
    i_hex = hex(i)[2:].rjust(16,'0')
    # 每两位进行划分, 满足可执行程序调用需求
    plain_text = split_string(i_hex, 2)
    # 调用DES加密程序
    subprocess.call(['./des_enc1',plain_text[0], plain_text[1], plain_text[2], plain_text[3], plain_text[4], plain_text[5], plain_text[6], plain_text[7]])