function fine_vocabularies(file_path,data_name,K,M,s)
addpath ('~/Documents/yael/yael_v401/matlab');
run('~/Downloads/software/vlfeat-0.9.20/toolbox/vl_setup');

all_data = fvecs_read(['~/Downloads/Dataset/' file_path '/learn/' data_name '_learn_' num2str(s) '.fvecs']);

tic;
type='NP';
vocabSize = 2^K;
R_opq_p = fvecs_read(['~/Documents/MultiIndex-master-OPQ/R_init/' data_name '_' num2str(vocabSize) type '.fvecs']);
all_data = uint8(R_opq_p' * all_data);


load(['~/Documents/MultiIndex-master-OPQ/coarse_quantization/' data_name '_double_' num2str(vocabSize) '_' type '.mat'], 'vocab1', 'vocab2');

vocab1 = int32(vocab1);
vocab2 = int32(vocab2);
i1 = vl_ikmeanspush((all_data(1:end/2,:)), vocab1);
i2 = vl_ikmeanspush((all_data(end/2+1:end,:)), vocab2);  
residual = single(all_data)- single([vocab1(:,i1); vocab2(:,i2)]);
bytes_per_point = M;

D = size(residual,1) / bytes_per_point;
residual_vocab = cell(bytes_per_point,1);
dist = cell(bytes_per_point,1);
niter = 30;
for m = 1:bytes_per_point
    chunk = residual(D*m-D+1:D*m,:);
        % add implementation of K-means
    residual_vocab{m} = yael_kmeans(chunk,2^K,'niter', niter, 'verbose', 0);
    dist{m} = vl_alldist2(residual_vocab{m});          
end

save(['~/Documents/MultiIndex-master-OPQ/fine_quantization/' data_name '_double_' num2str(vocabSize) '_' num2str(M) '_' type '.mat'],'residual_vocab','dist');

file = fopen(['~/Documents/MultiIndex-master-OPQ/fine_quantization/' data_name '_double_' num2str(vocabSize) '_' num2str(M) '_' type '.dat'], 'w');
vocabs_count = size(residual_vocab, 1);
each_vocab_count = size(residual_vocab{1}, 2);
each_vocab_dim = size(residual_vocab{1}, 1);
fwrite(file, vocabs_count, 'int32');
fwrite(file, each_vocab_count, 'int32');
fwrite(file, each_vocab_dim, 'int32');
for i = 1:vocabs_count
    for j = 1:each_vocab_count
        a = residual_vocab{i}(:,j);
        fwrite(file, a, 'float');
    end
end
fclose(file);

time=toc;

fid=fopen('preprocess.txt','a');
fprintf(fid,'%.6f #fine_time #K_%d #M_%d %s \n',time, K,M, data_name);

end
