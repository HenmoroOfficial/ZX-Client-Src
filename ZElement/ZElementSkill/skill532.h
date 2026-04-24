#ifndef __CPPGEN_GNET_SKILL532
#define __CPPGEN_GNET_SKILL532
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill532:public Skill
    {
      public:
        enum
        { SKILL_ID = 532 };
          Skill532 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill532Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill532Stub ():SkillStub (532)
        {
            occupation = 16;
            name = L"ÔÆÓê";
            nativename = "ÔÆÓê";
            icon = "ÔÆÓê.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 1;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 744;
            talent[1] = 748;
            talent[2] = 752;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[1] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[2] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[3] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[4] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[5] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill532Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 530 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetLevel () * skill->GetLevel () - 3.3 * skill->GetLevel () + 337);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 115, 117, 119, 121, 123, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (skill->GetLevel () * skill->GetLevel () - 3.3 * skill->GetLevel () + 337),
                               2 * skill->GetLevel (), 33, 4, 320 + 80 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * ((33 + 10 * skill->GetT2 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount ((320 + 80 * skill->GetLevel ()) *
                                            (3 / (1 + 2.0 * skill->GetPlayer ()->GetMp () / (skill->GetPlayer ()->GetMaxmp () + 1))) * (1 +
                                                                                                                                        0.4 *
                                                                                                                                        skill->
                                                                                                                                        GetT2 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (12000 * (1 + 0.3 * skill->GetT0 ()) + 100);
            skill->GetVictim ()->SetRatio ((0.14 + 0.03 * skill->GetT1 ()) * (INT (zrand (100) / (100 - 4 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetInchurt2 (1);
            return true;
        }
#endif
    };
}
#endif
