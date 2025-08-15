# Poseidon2 哈希算法电路实现

## 概述
本项目使用 `circom` 实现了 `Poseidon2` 哈希算法电路，并生成 `Groth16` 证明。

## 依赖项
- Node.js
- circom
- circomlib
- snarkjs

## 安装依赖
```bash
npm init -y
npm install circom circomlib snarkjs
```

## 电路文件
电路文件 `poseidon2.circom` 实现了 `Poseidon2` 哈希算法，参数为 `(n, t, d) = (256, 3, 5)`。

## 编译电路
```bash
npx circom poseidon2.circom --r1cs --wasm --sym
```

## 生成可信设置文件
```bash
npx snarkjs powersoftau new bn128 12 pot12_0000.ptau -v
npx snarkjs powersoftau contribute pot12_0000.ptau pot12_0001.ptau --name="First contribution" -v
npx snarkjs powersoftau beacon pot12_0001.ptau pot12_final.ptau 0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f 10 -n="Final"
npx snarkjs powersoftau verify pot12_final.ptau
```

## 生成 Groth16 密钥文件
```bash
npx snarkjs groth16 setup poseidon2.r1cs pot12_final.ptau poseidon2_0000.zkey
```

## 生成证明
```bash
npx snarkjs groth16 prove poseidon2_0000.zkey poseidon2_js/poseidon2.wasm input.json proof.json public.json
```

## 验证证明
```bash
npx snarkjs groth16 verify verification_key.json public.json proof.json
```

## 输入文件示例
`input.json` 文件示例：
```json
{
  "preimage": "1234567890"
}
```

## 注意事项
- 确保所有文件路径正确。
- 可信设置文件生成后需验证其有效性。