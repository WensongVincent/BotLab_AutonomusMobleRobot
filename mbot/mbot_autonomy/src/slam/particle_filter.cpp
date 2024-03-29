#include <utils/grid_utils.hpp>
#include <slam/particle_filter.hpp>
#include <slam/occupancy_grid.hpp>
#include <mbot_lcm_msgs/pose_xyt_t.hpp>
#include <mbot_lcm_msgs/particle_t.hpp>
#include <common_utils/geometric/angle_functions.hpp>
#include <cassert>

#include <utils/grid_utils.hpp>
#include <common_utils/geometric/point.hpp>
// #include <chrono>
// #include <sys/time.h>
// #include <ctime>
// using std::chrono::duration_cast;
// using std::chrono::milliseconds;
// using std::chrono::system_clock;

ParticleFilter::ParticleFilter(int numParticles)
: kNumParticles_ (numParticles),
  samplingAugmentation(0.5, 0.9, numParticles),
  distribution_quality(1),
  quality_reinvigoration_percentage(0.1)
{
    assert(kNumParticles_ > 1);
    posterior_.resize(kNumParticles_);
}

bool comparePose(mbot_lcm_msgs::particle_t p1, mbot_lcm_msgs::particle_t p2) {
    return (p1.weight > p2.weight);
}

void ParticleFilter::initializeFilterAtPose(const mbot_lcm_msgs::pose_xyt_t& pose)
{
    ///////////// TODO: Implement your method for initializing the particles in the particle filter /////////////////
    double sampleWeight = 1.0 / kNumParticles_;
    posteriorPose_ = pose;

    for (auto& particle : posterior_) {
        particle.pose.x = posteriorPose_.x;
        particle.pose.y = posteriorPose_.y;
        particle.pose.theta = wrap_to_pi(posteriorPose_.theta);
        particle.pose.utime = pose.utime;
        particle.parent_pose = particle.pose;
        particle.weight = sampleWeight;
    }
}

void ParticleFilter::initializeFilterRandomly(const OccupancyGrid& map)
{
    ///////////// TODO: Implement your method for initializing the particles in the particle filter /////////////////
    // Random stuff
    std::random_device rd;
    std::mt19937 gen(rd());
    double mean = 0.0, stddev = 1.0;
    std::normal_distribution<double> d(mean, stddev);

    double sampleWeight = 1.0 / kNumParticles_;
    Point<double> global_point; // initialize variables for checking map
    Point<int> map_point;
    int score = -1;
    
    for (auto& particle : posterior_) {
        while (score < 0) {
            global_point.x = d(gen); global_point.y = d(gen); // generate random x, y
            map_point = global_position_to_grid_cell(global_point, map); // get grid cell position
            score = map.logOdds(map_point.x, map_point.y); // check the score
        }
        particle.pose.x = global_point.x;
        particle.pose.y = global_point.y;
        particle.pose.theta = wrap_to_pi(d(gen));
        particle.pose.utime = posteriorPose_.utime;
        particle.weight = sampleWeight;
    }
    


    // // sample stuff
    // mbot_lcm_msgs::particle_t sample; // make a sample, assigning its parent_pose and pose, push it kNumParticles_ times
    // mbot_lcm_msgs::pose_xyt_t pose; // make a pose, set it to 0, assign it to sample.parent_pose
    // pose.x = 0; pose.y = 0; pose.theta = 0;
    // posteriorPose_ = pose;
    // actionModel_.resetPrevious(posteriorPose_);
    // sample.parent_pose = posteriorPose_;
    // sample.weight = 1.0/kNumParticles_;
    // std::cout << "InitializeFilterRandomly: " << sample.weight << std::endl;
    // Point<double> global_point; // initialize variables for checking map
    // Point<int> map_point;
    // int score;

    // while (posterior_.size() < kNumParticles_) {
    //     global_point.x = d(gen); global_point.y = d(gen); // generate random x, y
    //     map_point = global_position_to_grid_cell(global_point, map); // get grid cell position
    //     score = map.logOdds(map_point.x, map_point.y); // check the score

    //     if (score < 0) { // if the score is less than 0, then it's unoccupied based on laser scan, so we can place robot there (if >= 0, either not in map or occupied)
    //         pose.x = global_point.x; pose.y = global_point.y; pose.theta = d(gen)/10; // divide by a number since it's theta
    //         posterior_.push_back(sample);
    //     }
    // }
}

void ParticleFilter::resetOdometry(const mbot_lcm_msgs::pose_xyt_t& odometry)
{
    actionModel_.resetPrevious(odometry);
}


mbot_lcm_msgs::pose_xyt_t ParticleFilter::updateFilter(const mbot_lcm_msgs::pose_xyt_t& odometry,
                                                        const mbot_lcm_msgs::lidar_t& laser,
                                                        const OccupancyGrid& map)
{
    bool hasRobotMoved = actionModel_.updateAction(odometry);

    if(hasRobotMoved)
    {
        // auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // std::cout << "\n" << millisec_since_epoch << "\n";
        auto prior = resamplePosteriorDistribution(&map); // is map needed here? Not in Gaskell's example
        auto proposal = computeProposalDistribution(prior);
        posterior_ = computeNormalizedPosterior(proposal, laser, map);
        // OPTIONAL TODO: Add reinvigoration step
        posteriorPose_ = estimatePosteriorPose(posterior_);
        std::cout << odometry.x << ", " << odometry.y << ", " << odometry.theta << ", " << posteriorPose_.x << ", " << posteriorPose_.y << ", " << posteriorPose_.theta << "\n";


        // auto millisec_since_epoch2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // std::cout << millisec_since_epoch2 << "\n";
    }
    posteriorPose_.utime = odometry.utime;
    // std::cout<<"posteriorPose_.utime after reset = "<<posteriorPose_.utime<<"\n";
    return posteriorPose_;
}

mbot_lcm_msgs::pose_xyt_t ParticleFilter::updateFilterActionOnly(const mbot_lcm_msgs::pose_xyt_t& odometry)
{
    // Only update the particles if motion was detected. If the robot didn't move, then
    // obviously don't do anything.
    bool hasRobotMoved = actionModel_.updateAction(odometry);
    // std::cout << "posterior_.size = " << posterior_.size() << "\n";
    // std::cout << "first posterior x = : " << posterior_[0].pose.x << ", first posterior y = : " << posterior_[0].pose.y << ", first posterior theta = : " << posterior_[0].pose.theta << std::endl;
    
    if(hasRobotMoved)
    {
        auto prior = resamplePosteriorDistribution();
        auto proposal = computeProposalDistribution(prior);
        posterior_ = proposal;
    }

    posteriorPose_ = odometry;

    return posteriorPose_;
}



mbot_lcm_msgs::pose_xyt_t ParticleFilter::poseEstimate(void) const
{
    return posteriorPose_;
}


mbot_lcm_msgs::particles_t ParticleFilter::particles(void) const
{
    mbot_lcm_msgs::particles_t particles;
    particles.num_particles = posterior_.size();
    particles.particles = posterior_;
    return particles;
}


ParticleList ParticleFilter::resamplePosteriorDistribution(const OccupancyGrid* map)
{
    //////////// TODO: Implement your algorithm for resampling from the posterior distribution ///////////////////
    ParticleList prior;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> d(0.0, 1.0 / kNumParticles_);
    const double r = d(gen);  // std::cout << "r = " << r << "\n";
    double U = 0.0;
    double c = posterior_[0].weight;
    // std::cout << "Posterior_[0].weight = " << posterior_[0].weight << std::endl;
    int i = 0;

    for (int m=1; m<=kNumParticles_; m++) {
        U = r + (m-1.0) / kNumParticles_;
        // std::cout << "U: " << U << std::endl;
        while (U > c) {
            i += 1;
            c += posterior_[i].weight;
            // std::cout << "C: " << c << std::endl;
        }
        // std::cout << "posterior x = : " << posterior_[i].pose.x << ", posterior y = : " << posterior_[i].pose.y << ", posterior theta = : " << posterior_[i].pose.theta << std::endl;
        // std::cout << "i = " << i << "\n";
        prior.push_back(posterior_[i]);
    }
    return prior; // prior;
}


ParticleList ParticleFilter::computeProposalDistribution(const ParticleList& prior)
{
    //////////// TODO: Implement your algorithm for creating the proposal distribution by sampling from the ActionModel
    ParticleList proposal;
    for (auto& particle : prior) {
        // std::cout << "proposal particle pose = " << particle.pose.x << ", " << particle.pose.y << ", " << particle.pose.theta << "\n";
        proposal.push_back(actionModel_.applyAction(particle));
    }
    return proposal;
}


ParticleList ParticleFilter::computeNormalizedPosterior(const ParticleList& proposal,
                                                        const mbot_lcm_msgs::lidar_t& laser,
                                                        const OccupancyGrid& map)
{
    /////////// TODO: Implement your algorithm for computing the normalized posterior distribution using the
    ///////////       particles in the proposal distribution
    ParticleList posterior; // initilize new particle set
    double eta; // initialize the normalization factor
    
    for (auto& particle : proposal) {
        // apply sensor model to compute importance weight
        double weight = sensorModel_.likelihood(particle, laser, map);
        // std::cout << "sensorModel_likelihood = " << weight << "\n\n";

        // update normalization factor
        eta += weight; 
        
        // add to new particle set
        mbot_lcm_msgs::particle_t newParticle;
        newParticle.pose = particle.pose;
        newParticle.weight = weight;
        posterior.push_back(newParticle); 
    }
    
    for (auto& particle : posterior) {
        // normalize weight
        particle.weight /= eta; 
        // std::cout << "weight = " << particle.weight << "\n";
    }
    return posterior;
}


mbot_lcm_msgs::pose_xyt_t ParticleFilter::estimatePosteriorPose(const ParticleList& posterior)
{
    //////// TODO: Implement your method for computing the final pose estimate based on the posterior distribution
    mbot_lcm_msgs::pose_xyt_t bestPose;
    mbot_lcm_msgs::particle_t bestParticle;
    // double highestWeight = 0;
    // for (auto particle : posterior) {
    //     if (particle.weight > highestWeight) {
    //         bestParticle = particle;
    //         highestWeight = bestParticle.weight;
    //     }
    // }
    // std::cout << "highestWeight = " << highestWeight << "\n";
    // std::cout << "bestParticle.pose = " << bestParticle.pose.x << ", " << bestParticle.pose.y << ", " << bestParticle.pose.theta << " .\n";
    
    ParticleList posterior_sorted = posterior;
    std::sort(posterior_sorted.begin(), posterior_sorted.end(), comparePose);
    posterior_sorted.resize(50);
    bestPose = computeParticlesAverage(posterior_sorted);
    // std::cout << "bestPose = " << bestPose.x << ", " << bestPose.y << ", " << bestPose.theta << " \n";
    // for (int i = 0; i < 10; i++) {
    //     std::cout << "posterior_sorted[" << i << "].weight = " << posterior_sorted[i].weight << "\n";
    // }
    
    return bestPose;
}

mbot_lcm_msgs::pose_xyt_t ParticleFilter::computeParticlesAverage(const ParticleList& particles_to_average)
{
    //////// TODO: Implement your method for computing the average of a pose distribution
    // whether this should be weighted average?
    mbot_lcm_msgs::pose_xyt_t avg_pose;
    double sum_of_weights = 0.0;
    // for (auto& )
    double xAvg = 0.0;
    double yAvg = 0.0;
    double cosThetaAvg = 0.0;
    double sinThetaAvg = 0.0;
    for (auto& particle : particles_to_average) {
        sum_of_weights += particle.weight;
        xAvg += particle.weight * particle.pose.x;
        yAvg += particle.weight * particle.pose.y;
        cosThetaAvg += particle.weight * std::cos(particle.pose.theta);
        sinThetaAvg += particle.weight * std::sin(particle.pose.theta);
        // std::cout << "particle.pose = " << particle.pose.x << ", " << particle.pose.y << ", " << particle.pose.theta << ", Particle weight = " << particle.weight << "\n";
        // std::cout << "xAvg = " << xAvg << ", yAvg = " << yAvg << ", cosThetaAvg = " << cosThetaAvg << ", sinThetaAvg = " << sinThetaAvg << "\n"; 
    }
    xAvg /= sum_of_weights;
    yAvg /= sum_of_weights;
    cosThetaAvg /= sum_of_weights;
    sinThetaAvg /= sum_of_weights;
    avg_pose.x = xAvg;
    avg_pose.y = yAvg;
    avg_pose.theta = std::atan2(sinThetaAvg, cosThetaAvg); 
    return avg_pose;
}