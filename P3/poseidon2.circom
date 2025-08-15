// Poseidon2 哈希算法电路
// 参数：(n, t, d) = (256, 3, 5)

// 引入 Poseidon2 模板
include "node_modules/circomlib/circuits/poseidon.circom";

// 主电路
template Poseidon2Hash() {
    signal input preimage; // 隐私输入：哈希原象
    signal output hash;    // 公开输出：哈希值

    // 实例化 Poseidon2 哈希函数
    component poseidon = Poseidon(1); // 输入块数为 1

    // 连接输入
    poseidon.inputs[0] <== preimage;

    // 连接输出
    hash <== poseidon.out;
}

// 实例化主电路
component main = Poseidon2Hash();